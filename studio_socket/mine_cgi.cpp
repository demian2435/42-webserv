/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mine_cgi.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By:  <sgiovo>                                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 16:30:33 by sgiovo            #+#    #+#             */
/*   Updated: 2021/06/14 18:51:08 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#pragma once
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <sstream>

class CgiManager
{
private:
    int         pid;
    std::string ip;
    int         port;
    char        **env;
public:
    CgiManager(void){}
    CgiManager(std::string _ip, int _port, char **_env):pid(0),ip(_ip),port(_port),env(_env){}
    ~CgiManager()
    {
        if(pid == -5)
            return;
        killah();
        pid = -5;
    }
    void        start_php()
    {
        char **cmd = (char **)malloc(sizeof(char *) * 4);
        cmd[0] = strdup("/usr/bin/php");
        cmd[1] = strdup("-S");
        std::stringstream ss;
        ss << ip << ":" << port;
        cmd[2] = strdup(ss.str().c_str());
        cmd[3] = 0;
        pid = fork();
        if (!pid)
        {
            execve("/usr/bin/php", cmd, env);
            std::cout << "FATAL ERROR" << std::endl;
        }
        for (int i = 0; i < 3; i++)
            free(cmd[i]);
        free(cmd);
    }
    int         getPid(void)const{return this->pid;}
    void        killah(void){kill(pid, SIGTERM);}
    void        setIpPort(std::string );

};

int main(int argc, char **argv, char **env)
{
    CgiManager m("127.0.0.1", 9000, env);
    m.start_php();
    std::string c;
    std::cin >> c;
}