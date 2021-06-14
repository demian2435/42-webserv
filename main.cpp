#include "Config.hpp"
#include "studio_socket/Request.hpp"
#include "studio_socket/Response.hpp"
Config    test_config()
{
	Config conf("./studio_socket/webserv.conf");

	for (size_t i = 0; i < conf.server.size(); i++)
	{
		if (i %2 == 0)
			std::cout << RED;
		else
			std::cout << YELLOW;
		std::cout << "HOST: " << conf.server[i].host << std::endl;
		std::cout << "NAME: " << conf.server[i].name << std::endl;
		std::cout << "PORT: " << conf.server[i].port << std::endl;
		for (size_t j = 0; j < 700; j++)
		{
			//puts("rrrrrrr");
			if (conf.server[i].error_pages.getPath(j) != "")
				std::cout << "ERROR: " << j << " " << conf.server[i].error_pages.getPath(j) << std::endl;
		}
		for (size_t j = 0; j < conf.server[i].location.size(); j++)
		{
			std::cout << "	PATH: " << conf.server[i].location[j].path << std::endl;
			std::cout << "	ROOT: " << conf.server[i].location[j].root << std::endl;
			std::cout << "	CGI-PATH: " << conf.server[i].location[j].cgi_path << std::endl;
			std::cout << "	CGI-EXTENSION: " << conf.server[i].location[j].cgi_extension << std::endl;
			std::cout << "	MAX BODY: " << conf.server[i].location[j].client_max_body_size << std::endl;
			for (size_t x = 0; x < conf.server[i].location[j].index.size(); x++)
				std::cout << "		INDEX: " << conf.server[i].location[j].index[x] << std::endl;	
		}
		std::cout << RESET;	
	}
	return conf;
}

Request request_test()
{
    std::string req = "GET / HTTP/1.1\nHost: localhost:8080\nReferer: http://localhost:8080/ciccio/html\nConnection: keep-alive\n\n";
    Request r((char *)(req.c_str()));
    r.print_request();
    return r;
}

int main()
{
	// std::string str = "ciao belol beololvoflvc";
	test_config();
    // config c;
    // Request r;
    // c = test_config();
    // r = request_test();
    // Response tmp(c, r);
    // std::cout << tmp.out;

}