#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#define PORT 8080

int main(int argc, char **argv)
{
	int sock = 0;
	int valread = 0;
	struct sockaddr_in serv_addr;
	std::string hello;
	std::cout << argv[1] << std::endl;
	if (argc == 2)
		hello = std::string(argv[1]);
	else
		hello = std::string("CIAO IO SONO IL CLIENT");


	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		exit(1);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
		exit(1);

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		exit(1);

	send(sock , hello.c_str() , hello.length() , 0 );
	std::cout << "Messaggio inviato al server" << std::endl;

	valread = read( sock , buffer, 1024);
	std::cout << "Messaggio in arrivo dal server: " << buffer << std::endl;
	return 0;
}
