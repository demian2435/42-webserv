#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#define PORT 8080
#define BUFFER_SIZE 1024

int main(void)
{
	int server_fd;
	int new_socket;
	int valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE];
	for (int i = 0; i < BUFFER_SIZE; i++)
		buffer[i] = 0;
	std::string hello = "MESSAGGIO RICEVUTO DAL SERVER";
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		exit(1);
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
		exit(1);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
		exit(1);

	while(1)
	{
		if (listen(server_fd, 3) < 0)
			exit(1);

		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
			exit(1);

		valread = read( new_socket , buffer, BUFFER_SIZE);
		std::cout << "Messaggio in arrivo dal client: " << buffer << std::endl;
		send(new_socket , hello.c_str() , hello.length() , 0 );
		std::cout << "Messaggio inviato al client\n\n" << std::endl;
		for (int i = 0; i < BUFFER_SIZE; i++)
			buffer[i] = 0;
		close(new_socket);
	}
	return 0;
}
