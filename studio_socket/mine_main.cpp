#include "CgiManager.hpp"


int main(int argc, char **argv, char **env)
{
    CgiManager cm(env);
    cm.solve_php("./phpshit.php");
    return 0;
}