#include "Server.hpp"

int main(int argc, char **argv)
{
	srand((unsigned)time(NULL) * getpid());
	Server *s;
	if (argc == 2)
		s = new Server(argv[1]);
	else
		s = new Server("./webservDoppio.conf");
	s->start();
	delete s;
}

