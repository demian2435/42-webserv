#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#define PORT 8080
#define BUFFER_SIZE 20000

int main(void)
{
	//server's file descriptor in witch the clients will write requests
	int server_fd;

	//the temp socket created to catch a client request and respond
	int new_socket;

	//used to take response from the read function in the while loop, seems to be unused
	int valread;

	//socket address configuration
	struct sockaddr_in address;

	//used in the function setsockopt, but not sure about the use
	int opt = 1;

	//size of address, not sure of why is using a variable to store it
	int addrlen = sizeof(address);

	//buffer di lettura
	char buffer[BUFFER_SIZE];

	//azzera il buffer statico, gli array statici sono inizializzati a 0, pe ril momento commentato
		//for (int i = 0; i < BUFFER_SIZE; i++)
		//	buffer[i] = 0;

	//hello world response
	std::string hello = "HTTP/1.1 200 OK\n"
						"Content-Length: 34\n"
						"Content-Type: text/html\n"
						"Connection: Closed\n"
						"\n<html>"
							"<h1>Hello world!</h1>"
						"</html>\n";
	
	//creazione dell' fd attraverso la funzione socket
	//AF_INET indica l'uso del protocollo ipv4
	//SOCK_STREAM indica l'uso del protocollo TCP
	//lo zero dovrebbe essere una rappresentazione numerica del protocollo
	//indagare sul perchè sia proprio zero
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		exit(1);
	
	//This helps in manipulating options for the socket referred by the file descriptor sockfd. 
	//This is completely optional, but it helps in reuse of address and port.
	//Prevents error such as: “address already in use”.
	//int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		exit(1);
	
	//set sin_family to IPV4
	address.sin_family = AF_INET;
	//set address to localhost
	address.sin_addr.s_addr = INADDR_ANY;
	//set the listening port
	address.sin_port = htons( PORT );
	
	//binds the socket to the address localhost
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
		exit(1);

	while(1)
	{
		if (listen(server_fd, 3) < 0)
			exit(1);

		/*
		Await a connection on socket FD.
		When a connection arrives, open a new socket to communicate with it,
		set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
		peer and *ADDR_LEN to the address's actual length, and return the
		new socket's descriptor, or -1 for errors.
		*/
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			exit(1);

		//read buffer with client message
		valread = read( new_socket , buffer, BUFFER_SIZE);
		std::cout << "Messaggio in arrivo dal client: " << buffer << std::endl;

		//QUI ANDREBBE L'ELABORAZIONE DELLA RICHIESTA

		//send the response
		send(new_socket , hello.c_str() , hello.length() , 0 );
		std::cout << "Messaggio inviato al client\n" << hello << std::endl;

		//it cleans the buffer to take a new message
		for (int i = 0; i < BUFFER_SIZE; i++)
			buffer[i] = 0;

		//close connection with requesting client
		close(new_socket);
	}
	return 0;
}
