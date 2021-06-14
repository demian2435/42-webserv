#include "Server.hpp"

int main(void)
{
    Server s1("webserv.conf", "127.0.0.1", 8020);
    s1.start();
}
