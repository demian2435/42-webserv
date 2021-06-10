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
#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CONNECTIONS 999
/* COLORS */
#define	GREEN "\033[0;32m"
#define OFF "\033[0m"

int main(void)
{
	//---------------VARIABILI----------------//
	config	conf;
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
	int yes = 1;
	// Risposta standard del server
	std::string hello = "HTTP/1.1 200 OK\n"
						"Content-Length: 34\n"
						"Content-Type: text/html\n"
						"Connection: Closed\n"
						"\n<html>"
							"<h1>Hello world!</h1>"
						"</html>\n";

	//----------------------PREPARAZIONE----------------------------//

	// Azzeriamo i set
	FD_ZERO(&temp_fd);
	FD_ZERO(&base_fd);

	// Generiamo l'FD per il listener
	if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cout << "ERRORE SOCKET" << std::endl;
		return (1);
	}
	// Settiamo il socket come NONBLOCK
	fcntl(listener, F_SETFL, O_NONBLOCK);

	// Impostiamo che il socket puo riutilizzare gli address
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		std::cout << "ERRORE SETSOCKOPT" << std::endl;
		return (1);
	}

	// Colleghiamo il listener alla porta selezionata
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	if (bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		std::cout << "ERRORE BIND" << std::endl;
		return (1);
	}

  	// Prepariamo il listener a ricevere connessioni
	if (listen(listener, MAX_CONNECTIONS) == -1) {
		std::cout << "ERRORE LISTEN" << std::endl;
		return (1);
	}

 	// Aggiungiamo il listener al set di base
 	FD_SET(listener, &base_fd);
	// Inizializziamo l'fdTot
	fdTot = listener;

	//---------------MAIN LOOP----------------//
	std::cout << GREEN << "SERVER ATTIVO" << OFF << std::endl;
	while (true)
	{
		// Copiamo il set di base dentro al set degli fd in arrivo cosi da vedere se il numero è maggiore di 1
		// il che significherà che ci sono client in attesa
		temp_fd = base_fd;
		// Selezioniamo il primo client che ci ha chiamati (ovviamente sarà l'index 1 poichè allo 0 c'è il server)
		if (select(fdTot+1, &temp_fd, NULL, NULL, NULL) == -1) {
			std::cout << "ERRORE SELECT" << std::endl;
			return (1);
		}
		// Andiamo a interagire con tutti i client che ci hanno contattato
		for (int i = 0; i <= fdTot; i++) {
			// Andiamo a controllare se è un FD valido l'index i del set
			if (FD_ISSET(i, &temp_fd)) {
				// Se server generiamo la connessione con il client in attesa
				if (i == listener) {
					// Salviamo la grandezza dell'address del client
					addrlen = sizeof(clientAddr);
					// Acettiamo la conessione generando un nuovo FD
					if ((newfd = accept(listener, (struct sockaddr *)&clientAddr, &addrlen)) == -1) {
               			std::cout << "ERRORE ACCEPT" << std::endl;
             		} else {
						// Inseriamo il nuovo FD nella lista base
						FD_SET(newfd, &base_fd);
						// Aggiorniamo il totale di FD aperti
						if (newfd > fdTot) {
                 			fdTot = newfd;
               			}
						std::cout << "Nuova connessione da " << inet_ntoa(clientAddr.sin_addr) << " sul socket " << newfd << std::endl;
					}
				}
				else // Se Client adiamo a connetterci con i client 
				{
					// Leggiamo il buffer inviatoci dal client (In caso 0 o -1 chiudiamo la connessione)
					if ((nbytes = recv(i, buff, sizeof(buff), 0)) <= 0) {
						// Conessione chiusa dal client se 0 size
						if (nbytes == 0) {
							std::cout << "Connessione chiusa da socket " << newfd << std::endl;
						} else {
							std::cout << "ERRORE RECV" << std::endl;
						}
						// Chiudiamo la connessione
						close (i);
						// Eliminiamo l'FD dal set base
						FD_CLR(i, &base_fd);
					}
					else // Il server risponde ai Client
					{
						// Controlliamo se ci sono Client che attendono una risposta
						for(int j = 0; j <= fdTot; j++) {
							// Controlliamo se è presente nella lista delle consseioni accettate
							if (FD_ISSET(j, &base_fd)) {
								// Bypassiamo il server
								if (j != listener) {
									// Stampiamo il buffer del client
									std::cout << "Messaggio del client " << j << ": ";
									for (int k = 0; k < nbytes; k++)
										std::cout << buff[k];
									std::cout << std::endl;
									Request req(buff);
									// Mandiamo la risposta al client
									Response resp(conf, req);
									if (send(j, resp.out.c_str(), resp.out.length(), 0) == -1) {
										std::cout << "ERRORE SEND" << std::endl;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return (0);
}
