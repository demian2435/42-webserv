#include "MultiServer.hpp"
#include <vector>

int main(void)
{
    Server s1("127.0.0.1", 8080);
    s1.start();
}
