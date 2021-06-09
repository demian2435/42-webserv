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
    std::string req = "GET /favicon.ico HTTP/1.1\nHost: localhost:8080\nConnection: keep-alive\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.77 Safari/537.36\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\nSec-Fetch-Site: same-origin\nSec-Fetch-Mode: no-cors\nSec-Fetch-Dest: image\nReferer: http://localhost:8080/vcvcvcv\nAccept-Encoding: gzip, deflate, br\nAccept-Language: it-IT,it;q=0.9,en-US;q=0.8,en;q=0.7";
    Request r((char *)(req.c_str()));
}