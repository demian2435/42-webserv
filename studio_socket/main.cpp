#include "Server.hpp"
#include <vector>

int main(void)
{
    Server s1("webserv.conf");
    s1.start();
}
