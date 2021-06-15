#include "Server.hpp"

int main(void)
{
	Server s1("./webserv.conf");
	s1.start();
}
