#include "config/conf_parsing.hpp"

int main()
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
        //it2 = it->getIndex().begin();
        //while(it2 != it->getIndex().end())
        //{
        //    //std::cout << "  " << it2 << std::endl;
        //    it2++;
        //}
        std::cout << "  cgi_path:" << it->getCgiPath() << std::endl;
        std::cout << "  max body size: " << it->getMaxBodySize() << std::endl;
        it++;
    }
}