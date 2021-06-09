#include "config/conf_parsing.hpp"
#include "studio_socket/Request.hpp"
void    test_config()
{
    config  info;

    std::cout << info.name << std::endl;
    std::cout << info.host << std::endl;
    std::cout << info.port << std::endl;
    std::cout << "error pages:" << std::endl;
    for (size_t i = 0; i < 700; i++)
    {
        if (info.error_pages.getPath(i) != "")
            std::cout << "  " << i << " " << info.error_pages.getPath(i) << std::endl;
    }
    std::list<location>::iterator    it(info.locations.begin());
    std::list<std::string>::iterator   it2;

    while (it != info.locations.end())
    {
        std::cout << "path: " << it->getPath() << std::endl;
        std::cout << "  root: " << it->getRoot() << std::endl;
        std::cout << "  autoindex: " << it->getAutoindex() << std::endl;
        std::cout << "  cgi_extension: " << it->getCgiExtension() << std::endl;
        it2 = it->getIndex().begin();
        std::cout << "  Index: " << std::endl;
        for (size_t i = 0; i < it->getIndex().size(); i++)
        {
            std::cout << "      " << *it2 << std::endl;
            it2++;
        }
        
        std::cout << "  cgi_path:" << it->getCgiPath() << std::endl;
        std::cout << "  max body size: " << it->getMaxBodySize() << std::endl;
        it++;
    }
}

int main()
{
    std::string req = "POST /cgi-bin/process.cgi HTTP/1.1\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\nHost: www.tutorialspoint.com\nContent-Type: text/xml; charset=utf-8\nContent-Length: 600\nAccept-Language: en-us\nAccept-Encoding: gzip, deflate\nConnection: Keep-Alive\n\n<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<string xmlns=\"http://clearforest.com/\">string</string>";
    Request r((char *)(req.c_str()));
    r.print_request();
}