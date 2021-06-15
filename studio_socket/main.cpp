#include "Server.hpp"
#include "Request.hpp"
#include <vector>

// int main(void)
// {
//     Server s1("127.0.0.1", 8080);
//     s1.start();
// }

int main()
{

	std::string str = "POST /cgi-bin/process.cgi HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: localhost:8080\nContent-Type: application/x-www-form-urlencoded\nContent-Length: 900\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n\n";

	char *req;
	req = &str[0];
	Request r(req);
	r.print_request();
}