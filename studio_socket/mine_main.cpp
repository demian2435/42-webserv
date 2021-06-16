#include "CgiManager.hpp"


int main(int argc, char **argv, char **env)
{
    CgiManager cm;
    std::vector<std::string> vec;
    vec.push_back("name=ciao");
    std::string l = cm.solve_php_GET("www/phpshit.php", vec);
    std::cout << l << std::endl;
    return 0;
}