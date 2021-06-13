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
#include "conf_parsing.hpp"
#include "Response.hpp"
#include "Request.hpp"
/* COLORS */
#define GREEN "\033[0;32m"
#define OFF "\033[0m"

class Server
{
private:
	//---------------VARIABILI----------------//
	config conf;
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
	char buff[1024];
	// Lughezza buffer data
	int nbytes;
	// Variabile per impostare il setsockopt
	int yes = 1;
	// porta su cui montare il server
	int select_port = 8080;
	// indirizzo su cui montare il server
	std::string select_ip = "127.0.0.1";
	// Massimo connessioni consentite
	int max_connections = 9999;
	// Struttura che conterrà le variabili del timeout del server
	struct timeval timeout;
	// timeout in secondi per il server
	int t = 5;

	//----------------------PREPARAZIONE----------------------------//
public:
	Server(){};

	int setup()
	{
		// Azzeriamo i set
		FD_ZERO(&temp_fd);
		FD_ZERO(&base_fd);

		// Generiamo il socket listener
		// PF_INET = Ip Protocol Family e cioè Internet
		// SOCK_STREAM = TCP, quindi possiamo utilizzare send() e recv()
		// Il protocollo per PF_INET può essere posto a zero
		if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
		{
			std::cout << "ERRORE SOCKET" << std::endl;
			return (-1);
		}
		// Settiamo il socket come NONBLOCK
		fcntl(listener, F_SETFL, O_NONBLOCK);

		// Impostiamo che il socket puo riutilizzare gli address
		// SOL_SOCKET imposta le mofiche a livello socket
		// SO_REUSEADDR consente l'utilizzo di un indirizzo locale, cosi che
		// se un proesso figlio o padre già utilizza quell'address anche questa sessione
		// potrà utilizzare lo stesso address
		if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			std::cout << "ERRORE SETSOCKOPT" << std::endl;
			return (-2);
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
			return (-3);
		}

		// Abilitiamo il socket listener a ricevere connessioni
		// max_connections = massimo connessioni dopo il quale risponde con il rifiuto della connessione
		if (listen(listener, max_connections) == -1)
		{
			std::cout << "ERRORE LISTEN" << std::endl;
			return (-4);
		}

		// Aggiungiamo il listener al set di base
		FD_SET(listener, &base_fd);
		// Inizializziamo l'fdTot con il valore FD più alto attualmente e poiche abbiamo solo
		// listener questo corrispponderà esattamente a listener
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
		while (true)
		{
			std::cout << GREEN << "SERVER IN ATTESA" << OFF << std::endl;
			// Impostiamo il timeout del server, poiche ogni volta si azzera
			timeout.tv_sec = t;
			timeout.tv_usec = 0;
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
			if (select(fdTot + 1, &temp_fd, NULL, NULL, &timeout) == -1)
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
							Response resp(conf, req);
							std::cout << GREEN << resp.out << OFF << std::endl;
							if (send(i, resp.out.c_str(), resp.out.length(), 0) == -1)
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
};
