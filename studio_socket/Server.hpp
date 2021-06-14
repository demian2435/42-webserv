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
#include <vector>
#include "Config.hpp"
#include "Request.hpp"
#define BUFFER_SIZE 1024

class Server
{
private:
	//---------------VARIABILI----------------//
	Config conf;
	// Contenitore per i file descriptor che arriveranno
	fd_set temp_fd;
	// Contenitore di backup, cosi da controllare le differenze nel tempo
	fd_set base_fd;
	// Server address
	struct sockaddr_in serverAddr;
	// Client address
	struct sockaddr_in clientAddr;
	// Totale fd aperti al momento
	int fdTot;
	// Fd del listener
	int listener;
	// Nuovo fd accettato dal listner
	int newfd;
	// Variabile che contiene il size dell'address in arrivo
	socklen_t addrlen;
	// Buffer data
	char buff[BUFFER_SIZE];
	// Lughezza buffer data
	int nbytes;
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
	// variabile del while per il main loop
	bool play_loop;

	//----------------------PREPARAZIONE----------------------------//
public:
	Server(std::string _ip, int _port)
	{
		play_loop = true;
		fdTot = -1;
		for (int i = 0; i < BUFFER_SIZE; i++)
			buff[i] = 0;
		yes = 1;
		max_connections = 10;
		timeout = 30;
		
		select_port = _port;
		select_ip = _ip;

		// Azzeriamo i set
		FD_ZERO(&temp_fd);
		FD_ZERO(&base_fd);

		setup();
	};

	int setup()
	{
		// Generiamo il socket listener
		// PF_INET = Ip Protocol Family e cioè Internet
		// SOCK_STREAM = TCP, quindi possiamo utilizzare send() e recv()
		// Il protocollo per PF_INET può essere posto a zero
		if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cout << "ERRORE SOCKET" << std::endl;
			return (0);
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
			return (0);
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
			return (0);
		}

		// Abilitiamo il socket listener a ricevere connessioni
		// max_connections = massimo connessioni dopo il quale risponde con il rifiuto della connessione
		if (listen(listener, max_connections) == -1)
		{
			std::cout << "ERRORE LISTEN" << std::endl;
			return (0);
		}

		// Aggiungiamo il listener al set di base
		FD_SET(listener, &base_fd);
		// Inizializziamo l'fdTot con il valore FD più alto attualmente e poiche abbiamo solo
		// listener questo corrispponderà esattamente a listener
		if (listener > fdTot)
			fdTot = listener;

		return (1);
	}
	//---------------MAIN LOOP----------------//
	// Multiplazione del I/O: Il server riesce ad accettare nuove connessioni ed a servire quelle già attive
	// grazie all'uso della funzione Select(), meno elegante ma più rapido poiche non usa nuove risorse
	// thread o fork per gestire le nuove connessioni cosi da risparmiare tempo e spazio, ma il lato migliore
	// è l'assenza di qualsiasi errore di sincronizzazione, unico lato negativo e la perdita di leggibilità
	// del codice poichè vanno gestiti tutti i flussi in contemporanea
	int start()
	{
		while (play_loop)
		{
			std::cout << GREEN << "SERVER IN ATTESA" << RESET << std::endl;
			// Impostiamo il timeout del server, poiche ogni volta si azzera
			s_timeout.tv_sec = timeout;
			s_timeout.tv_usec = 0;
			// Copiamo il set di base dentro al set degli fd da monitorare poichè select lascia nel set solo
			// gli FD che hannno ricevuto un cambio di stato, eliminando il resto
			temp_fd = base_fd;
			// Select ritorna quando un qualsiasi socket dentro il SET da monitorare cambia stato
			// 0: il valore deve corrispondere all'FD più grande + 1
			// 1: è il set che contiene gli FD che attiveranno Select quando conterranno dati da leggere
			// 2: è il set che contiene gli FD che attiveranno Select quando dovranno accettare dati
			// 3: è il set degli FD che attiveranno Select quando genereranno condizioni di errore
			// 4: è il timeout, una volta esaurito la funzione ritorna, ovviamente se nulla è cambiato il successivo
			// codice non produrrà nessun risultato
			if (select(fdTot + 1, &temp_fd, NULL, NULL, &s_timeout) == -1)
			{
				std::cout << "ERRORE SELECT" << std::endl;
				return (1);
			}
			// Andiamo a interagire con tutti gli FD contenuti nel SET che hanno ricevuto un cambio di stato
			for (int i = 0; i <= fdTot; i++)
			{
				// Andiamo a controllare se l'index i è un FD contenuto nel Set che stiamo monitorando
				if (FD_ISSET(i, &temp_fd))
				{
					// Se è il server, significa che è stato contattato da una nuova connessione
					if (i == listener)
					{
						// Salviamo la grandezza dell'address del client
						addrlen = sizeof(clientAddr);
						// Acettiamo la conessione generando un nuovo FD
						if ((newfd = accept(listener, (struct sockaddr *)&clientAddr, &addrlen)) == -1)
						{
							std::cout << "ERRORE ACCEPT" << std::endl;
						}
						else
						{
							// Inseriamo il nuovo FD nella lista base
							FD_SET(newfd, &base_fd);
							// Se il nuovo FD supera il maxFd attuale lo aggiorniamo
							if (newfd > fdTot)
							{
								fdTot = newfd;
							}
							std::cout << "Nuova connessione da " << inet_ntoa(clientAddr.sin_addr) << " sul socket " << newfd << std::endl;
						}
					}
					else // Se è Client significa che vuole dirci qualcosa, poichè il set minitora se ci sono
						 // dati da leggere pendenti
					{
						// Leggiamo il buffer inviatoci dal client (In caso 0 o -1 chiudiamo la connessione)
						if ((nbytes = recv(i, buff, sizeof(buff), 0)) <= 0)
						{
							// Conessione chiusa dal client se 0 size
							if (nbytes == 0)
							{
								std::cout << "Connessione chiusa da socket " << newfd << std::endl;
							}
							else
							{
								std::cout << "ERRORE RECV" << std::endl;
							}
							// Chiudiamo la connessione
							close(i);
							// Eliminiamo l'FD dal set base
							FD_CLR(i, &base_fd);
							// Non abbiamo bisogno di aggiorare maxFd poichè non nuoce controllare qualche fd in più
						}
						else // Se l lettura è andata a buon fine parsiamo la richiesta e rispondiamo al client
						{
							// Stampiamo il buffer del client
							std::cout << "Messaggio del client " << i << ": ";
							for (int j = 0; j < nbytes; j++)
								std::cout << buff[j];
							std::cout << std::endl;
							Request req(buff);
							// Mandiamo la risposta al client
							// Response resp(conf, req);
							//std::cout << GREEN << resp.out << RESET << std::endl;
							std::string xxx = "HTTP/1.1 404 NOT FOUND\nContent-Type: text/html\nContent-Length: 41\nConnection: keep-alive\n<html><h1>ERROR 404 NOT FOUND</h1></html>";
							if (send(i, xxx.c_str(), xxx.length(), 0) == -1)
							{
								std::cout << "ERRORE SEND" << std::endl;
							}
						}
					}
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
};

