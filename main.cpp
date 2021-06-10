#include "studio_socket/conf_parsing.hpp"
#include "studio_socket/Request.hpp"
#include "studio_socket/Response.hpp"
config    test_config()
{
    config  info;

    // std::cout << info.name << std::endl;
    // std::cout << info.host << std::endl;
    // std::cout << info.port << std::endl;
    // std::cout << "error pages:" << std::endl;
    // for (size_t i = 0; i < 700; i++)
    // {
    //     if (info.error_pages.getPath(i) != "")
    //         std::cout << "  " << i << " " << info.error_pages.getPath(i) << std::endl;
    // }
    // std::list<location>::iterator    it(info.locations.begin());
    // std::list<std::string>::iterator   it2;

    // while (it != info.locations.end())
    // {
    //     std::cout << "path: " << it->getPath() << std::endl;
    //     std::cout << "  root: " << it->getRoot() << std::endl;
    //     std::cout << "  autoindex: " << it->getAutoindex() << std::endl;
    //     std::cout << "  cgi_extension: " << it->getCgiExtension() << std::endl;
    //     it2 = it->getIndex().begin();
    //     std::cout << "  Index: " << std::endl;
    //     for (size_t i = 0; i < it->getIndex().size(); i++)
    //     {
    //         std::cout << "      " << *it2 << std::endl;
    //         it2++;
    //     }
        
    //     std::cout << "  cgi_path:" << it->getCgiPath() << std::endl;
    //     std::cout << "  max body size: " << it->getMaxBodySize() << std::endl;
    //     it++;
    // }
    return info;
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
    config c;
    Request r;
    c = test_config();
    r = request_test();
    Response tmp(c, r);
    std::cout << tmp.out;

}