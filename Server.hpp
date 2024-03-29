#pragma once

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <fcntl.h>
#include <arpa/inet.h>
#include <map>
#include "Config.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "FileUpload.hpp"
#include "Client.hpp"
#define BUFFER_SIZE 5000000

class Server
{
private:
	//---------------VARIABILI----------------//
	Config conf;
	// Contenitore per i file descriptor che arriveranno
	fd_set temp_read_fd;
	// Contenitore per i file descriptor che aspetteranno una risposta
	fd_set temp_write_fd;
	// Contenitore di backup, cosi da controllare le differenze nel tempo
	fd_set base_read_fd;
	// Contenitore di backup, cosi da controllare le differenze nel tempo
	fd_set base_write_fd;
	// Contenitore per gestire multiport server
	fd_set server_fd;
	// Mappa per ritrovare il server dalla porta
	std::map<int, int> port_server;
	// Server address
	struct sockaddr_in serverAddr;
	// Client address
	struct sockaddr_in clientAddr;
	// Totale fd aperti al momento
	int fdMax;
	// Fd del listener
	int listener;
	// Nuovo fd accettato dal listner
	int newfd;
	// Variabile che contiene il size dell'address in arrivo
	socklen_t addrlen;
	// Buffer data
	char buff[BUFFER_SIZE];
	// Variabile per impostare il setsockopt
	int yes;
	// porta su cui montare il server
	int select_port;
	// indirizzo su cui montare il server
	std::string select_ip;
	// Massimo connessioni consentite
	int max_connections;
	// Struttura che conterrà le variabili del timeout del server
	struct timeval s_timeout;
	// timeout in secondi per il server
	int timeout;
	// mappa dei client attivi
	std::map<int, Client> client_map;

	//----------------------PREPARAZIONE----------------------------//
public:
	Server(){};
	Server(std::string _path)
	{
		fdMax = -1;
		for (int i = 0; i < BUFFER_SIZE; i++)
			buff[i] = 0;
		conf = Config(_path);
		yes = 1;
		max_connections = 100000;
		timeout = 30;
		
		// Azzeriamo i set
		FD_ZERO(&temp_write_fd);
		FD_ZERO(&temp_read_fd);
		FD_ZERO(&base_read_fd);
		FD_ZERO(&base_write_fd);

		for (size_t i = 0; i < conf.server.size(); i++)
		{
			select_port = conf.server[i].port;
			select_ip = conf.server[i].host;
			if (setup(i))
				exit(0);
		}
	};

	int setup(int index)
	{
		// Generiamo il socket listener
		// PF_INET = Ip Protocol Family e cioè Internet
		// SOCK_STREAM = TCP, quindi possiamo utilizzare send() e recv()
		// Il protocollo per PF_INET può essere posto a zero
		if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cout << "ERRORE SOCKET" << std::endl;
			return (1);
		}
		// Settiamo il socket come NONBLOCK
		// F_SETFL significa che vogliamo settare una flag
		// O_NONBLOCK significa che invece di bloccarsi il socket torna -1
		// cosi da reinviare la domanda a palla di fuoco, finché non si sblocca
		fcntl(listener, F_SETFL, O_NONBLOCK);

		// Impostiamo che il socket puo riutilizzare gli address
		// SOL_SOCKET imposta le mofiche a livello socket
		// SO_REUSEADDR consente l'utilizzo di un indirizzo locale, cosi che
		// se un proesso figlio o padre già utilizza quell'address anche questa sessione
		// potrà utilizzare lo stesso address
		if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			std::cout << "ERRORE SETSOCKOPT" << std::endl;
			return (1);
		}

		// Colleghiamo il listener all'indirizzo selezionato ma prima va inizializzata la struct dell'address
		// Utilizzeremo il protocollo IP come in precedenza
		serverAddr.sin_family = PF_INET;
		// Settiamo l'inidirizzo IP, la macro INADDR_ANY equivale a Localhost
		// la funzione inet_addr converte da stringa a in_addr_t
		// l'inverso si ottiene con inet_ntoa() che riceve la struct in_addr
		serverAddr.sin_addr.s_addr = inet_addr(select_ip.c_str());
		// Impostiamo la porta richiesta, se 0 sceglierà una porta in automatico
		// htons converte da host order a network order, Big endian || Little endian
		serverAddr.sin_port = htons(select_port);
		// Azzeriamo l'array sin_zero, non ho trovato molte info su questo, credo siano flag???
		memset(serverAddr.sin_zero, 0, sizeof(serverAddr.sin_zero));
		if (bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
		{
			std::cout << "ERRORE BIND" << std::endl;
			return (1);
		}

		// Abilitiamo il socket listener a ricevere connessioni
		// max_connections = massimo connessioni dopo il quale risponde con il rifiuto della connessione
		if (listen(listener, max_connections) == -1)
		{
			std::cout << "ERRORE LISTEN" << std::endl;
			return (1);
		}

		// Aggiungiamo il listener al set di base
		FD_SET(listener, &base_read_fd);
		// Per multi port server
		FD_SET(listener, &server_fd);
		// Inizializziamo l'fdMax con il valore FD più alto attualmente e poiche abbiamo solo
		// listener questo corrispponderà esattamente a listener
		if (listener > fdMax)
			fdMax = listener;

		// Aggiungo i dati alla mappa cosi da ritrovare il server conf attraverso l'FD
		port_server[listener] = index;

		return (0);
	}
	//---------------MAIN LOOP----------------//
	// Multiplazione del I/O: Il server riesce ad accettare nuove connessioni ed a servire quelle già attive
	// grazie all'uso della funzione Select(), meno elegante ma più rapido poiche non usa nuove risorse
	// thread o fork per gestire le nuove connessioni cosi da risparmiare tempo e spazio, ma il lato migliore
	// è l'assenza di qualsiasi errore di sincronizzazione, unico lato negativo e la perdita di leggibilità
	// del codice poichè vanno gestiti tutti i flussi in contemporanea
	int start()
	{
		while (true)
		{
			std::cout << GREEN << "SERVER IN ATTESA" << RESET << std::endl;
			// Impostiamo il timeout del server, poiche ogni volta si azzera
			s_timeout.tv_sec = timeout;
			s_timeout.tv_usec = 0;
			// Copiamo il set di base dentro al set degli fd da monitorare poichè select lascia nel set solo
			// gli FD che hannno ricevuto un cambio di stato, eliminando il resto
			temp_read_fd = base_read_fd;
			temp_write_fd = base_write_fd;
			// Select ritorna quando un qualsiasi socket dentro il SET da monitorare cambia stato
			// 0: il valore deve corrispondere all'FD più grande + 1
			// 1: è il set che contiene gli FD che attiveranno Select quando conterranno dati da leggere
			// 2: è il set che contiene gli FD che attiveranno Select quando dovranno accettare dati
			// 3: è il set degli FD che attiveranno Select quando genereranno condizioni di errore
			// 4: è il timeout, una volta esaurito la funzione ritorna, ovviamente se nulla è cambiato il successivo
			// codice non produrrà nessun risultato
			if (select(fdMax + 1, &temp_read_fd, &temp_write_fd, NULL, &s_timeout) == -1)
			{
				std::cout << "ERRORE SELECT" << std::endl;
				return (1);
			}
			// Andiamo a interagire con tutti gli FD contenuti nel SET che hanno ricevuto un cambio di stato
			for (int i = 0; i <= fdMax; i++)
			{
				// Andiamo a controllare se l'index i è un FD contenuto nel Set che stiamo monitorando
				if (FD_ISSET(i, &temp_read_fd))
				{
					// Se è il server, significa che è stato contattato da una nuova connessione
					if (FD_ISSET(i, &server_fd))
					{
						// Salviamo la grandezza dell'address del client
						addrlen = sizeof(clientAddr);
						// Acettiamo la conessione generando un nuovo FD
						if ((newfd = accept(i, (struct sockaddr *)&clientAddr, &addrlen)) == -1)
						{
							std::cout << "ERRORE ACCEPT" << std::endl;
						}
						else
						{
							// Inseriamo il nuovo FD nella lista base
							FD_SET(newfd, &base_read_fd);
							// Se il nuovo FD supera il maxFd attuale lo aggiorniamo
							if (newfd > fdMax)
								fdMax = newfd;

							Client c(i, port_server[i], newfd, addrlen, clientAddr);
							client_map[newfd] = c;

							std::cout << BLUE << "Nuova connessione da " << c.getIp() << " sul socket " << c.fd_client << RESET << std::endl;
						}
					}
					else // Se è Client significa che vuole dirci qualcosa, poichè il set minitora se ci sono
						 // dati da leggere pendenti
					{
						// Leggiamo il buffer inviatoci dal client (In caso 0 o -1 chiudiamo la connessione)
						client_map[i].nbytes = recv(i, buff, sizeof(buff), 0);

						if (client_map[i].nbytes > 0) // Se l lettura è andata a buon fine parsiamo la richiesta e rispondiamo al client
						{
							client_map[i].appendBuffer(buff);
						}
						if (client_map[i].header_ok() && client_map[i].isReady())
						{
							FD_CLR(i, &base_read_fd);
							FD_SET(i, &base_write_fd);
						}
						if (client_map[i].nbytes <= 0)
						{
							// Conessione chiusa dal client se 0 size
							std::cout << BLUE << "Connessione chiusa dal socket " << i << RESET << std::endl;
							// Chiudiamo la connessione
							close(i);
							// Eliminiamo l'FD dal set base
							FD_CLR(i, &base_read_fd);
							client_map.erase(i);
							// Non abbiamo bisogno di aggiorare maxFd poichè non nuoce controllare qualche fd in più
						}
					}
				}
				// Andiamo a controllare se l'index i è un FD contenuto nel Set che stiamo monitorando
				if (FD_ISSET(i, &temp_write_fd))
				{
					client_map[i].getRequest();
					std::cout << YELLOW << "REQUEST del client: " << i << RESET << std::endl;
					if (!(client_map[i].req.transfer_encoding.compare(0, 7, "chunked")))
						parse_chunked(client_map[i]);
					std::cout << client_map[i].getHeader() << std::endl;
					//std::cout << client_map[i].req.body <<std::endl;
					// Mandiamo la risposta al client,
					// per capire a quale server è stata inviata la richiesta andiamo a vedere nella mappa a quale configurazione equivale la porta della richiesta
					client_map[i].getResponse(conf);
					std::cout << YELLOW << "RESPONSE al client: " << i << RESET << std::endl;
					std::cout << client_map[i].res.out << std::endl;
					send(i, client_map[i].res.out.c_str(), client_map[i].res.out.length(), 0);
					if (client_map[i].req.upload && client_map[i].res.res_code == 200)
						FileUpload file(client_map[i].req);
					if (client_map[i].req.delete_file && client_map[i].res.res_code == 200)
					{
						std::string cmd;
						cmd.append("rm -rf ./upload/");
						cmd.append(client_map[i].req.filename);
						system(cmd.c_str());
					}
					std::cout << BLUE << "Chiudiamo la connessione al socket " << i << RESET << std::endl;
					// Chiudiamo la connessione
					close(i);
					// Eliminiamo l'FD dal set base
					FD_CLR(i, &base_write_fd);							
					client_map.erase(i);
					// Non abbiamo bisogno di aggiorare maxFd poichè non nuoce controllare qualche fd in più
				}
			}
		}
		return (0);
	}

	int restart()
	{
		return (0);
	}

	int stop()
	{
		return (0);
	}

	void		parse_chunked(Client &c)
	{
		int	size = 0;
		std::string	tmp_body;

		for (size_t i = 0; i < c.req.body.length(); i++)
		{
			std::string tmp;
			// 13 == \r
			while (c.req.body[i] != 13)
			{
				tmp += c.req.body[i];
				i++;
			}
			size = htoi(tmp);
			if (!size)
				break ;
			i += 2 ;
			for (size_t j = 0; j < (size_t)size; j++)
			{
				tmp_body += c.req.body[i];
				i++;
			}
		}
		c.req.body = tmp_body;
	}

	int		htoi(std::string hex)
	{
		std::stringstream x;
		int res;
		
		x << std::hex << hex;
		x >> res;
		return res;
	}
};

