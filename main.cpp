#include "./studio_socket/Config.hpp"
#include "./studio_socket/Response.hpp"
#include "./studio_socket/Request.hpp"
#include "FileUpload.hpp"

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
    std::string req = "POST / HTTP/1.1\nHost: localhost:8000\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:29.0) Gecko/20100101 Firefox/29.0\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\nAccept-Language: en-US,en;q=0.5\nAccept-Encoding: gzip, deflate\nCookie: __atuvc=34%7C7; permanent=0; _gitlab_session=226ad8a0be43681acf38c2fab9497240; __profilin=p%3Dt; request_method=GET\nConnection: keep-alive\nContent-Type: multipart/form-data; boundary=---------------------------9051914041544843365972754266\nContent-Length: 554\n\n-----------------------------9051914041544843365972754266\nContent-Disposition: form-data; name=\"text\"\n\ntext default\n-----------------------------9051914041544843365972754266\nContent-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\nContent-Type: text/plain\n\nContent of a.txt.\n\n-----------------------------9051914041544843365972754266\nContent-Disposition: form-data; name=\"file2\"; filename=\"a.html\"\nContent-Type: text/html\n\n<!DOCTYPE html><title>Content of a.html.</title>\n\n-----------------------------9051914041544843365972754266--";
    Request r((char *)(req.c_str()));
	if (r.content_type.compare(""))
    	FileUpload(r.body);
    return r;
}

int main()
{
	// std::string str = "ciao belol beololvoflvc";
	//test_config();
     Config c("./studio_socket/webserv.conf");
     Request r;
     //c = test_config();
     r = request_test();
    //  Response tmp(c.server[0], r);
    //  std::cout << tmp.out;

}