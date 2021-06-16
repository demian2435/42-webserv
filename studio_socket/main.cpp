#include "Server.hpp"

int main(void)
{
	Server s("./webservDoppio.conf");
	s.start();
}

