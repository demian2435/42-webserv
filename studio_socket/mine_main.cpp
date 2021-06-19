#include "Server.hpp"


int main(int argc, char **argv, char **env)
{
    CgiManager cm;
    Request req;
    req.path = "www/phpshit.php";
    req.body = "name=ciao&ciao=cacca";
    req.method = "POST";
    req.content_type = "application/x-www-form-urlencoded";
    std::vector<std::string> vec;
    vec.push_back("name=ciao");
//    std::string l = cm.solve_php_POST(req, "/Users/" + std::string(getenv("USER")) +  "/goinfre/._brew/bin/php-cgi");
    std::string l = CgiManager::solve_php_POST(req.path, req, "/Users/" + std::string(getenv("USER")) +  "/goinfre/.brew/bin/php-cgi");

    std::cout << l << std::endl << std::endl;


    std::cout << "--------------------------------------------" << std::endl << std::endl;
    return 0;
}