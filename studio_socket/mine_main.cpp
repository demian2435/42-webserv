#include "CgiManager.hpp"


int main(int argc, char **argv, char **env)
{
    CgiManager cm(env);
    std::string l = cm.solve_php("./phpshit.php");
    std::cout << l << std::endl;
    return 0;
}