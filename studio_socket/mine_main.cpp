#include "CgiManager.hpp"


int main(int argc, char **argv, char **env)
{
    CgiManager cm(env);
    cm.solve_php("./phpshit.php", "risultato.html");
    return 0;
}