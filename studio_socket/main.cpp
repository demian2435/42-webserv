#include "Server.hpp"

int main(void)
{
	srand((unsigned)time(NULL) * getpid());
	Server s("./webservDoppio.conf");
	s.start();
}

