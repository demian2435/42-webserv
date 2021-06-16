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

#pragma once
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <streambuf>

class CgiManager
{
private:
    char        **env;
public:
    CgiManager(void){}
    CgiManager(char **_env):env(_env){}
    ~CgiManager(){}
    std::string     solve_php(char const * path)
    {

        std::string method = "GET";
        std::string var = "a";
        std::string value = "3";
        std::string s = "$_" + method + "[" + var + "]" + "=" + value;
        //test dell orso

        pid_t pid;
        char **cmd = (char **)malloc(sizeof(char *) * 7);
        cmd[0] = strdup("/usr/bin/php");
        cmd[1] = strdup("-f");
        cmd[2] = strdup(path);
        cmd[3] = strdup("-B");
        cmd[4] = strdup(s.c_str());
        cmd[5] = 0;
        int fd = open("__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        pid = fork();
        std::stringstream buffer;
        if (!pid)
        {
            env[10] = strdup("ciao=\"ciao\"");
            std::cout << env[10] << std::endl;
            dup2(fd, STDOUT_FILENO);
            execve("/usr/bin/php", cmd, env);
            std::cout << "FATAL ERROR" << std::endl;
            std::cout << errno << std::endl;
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd);
            std::ifstream t("__DamSuperCarino__", std::ifstream::in);
            buffer << t.rdbuf();
            t.close();
        }
//        for (int i = 0; i < 3; i++)
//            free(cmd[i]);
//        free(cmd);
        return buffer.str();
    }
};

