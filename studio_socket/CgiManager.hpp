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
    char **env;
public:
    CgiManager(void){}
    ~CgiManager(){}
    std::string     solve_php(char const * path)
    {

        std::string method = "GET";
        std::string var = "ciao";
        std::string value = "3";
        std::string s = "$_" + method + "[" + var + "]" + "=" + value;
        //test dell orso

        pid_t pid;
        char **cmd = (char **)malloc(sizeof(char *) * 4);
        cmd[0] = strdup("~/goinfre/._brew/bin");
        //cmd[1] = strdup("-f");
        cmd[1] = strdup(path);
        //cmd[3] = strdup("-B");
        cmd[2] = strdup("ciao=5orreta");
        cmd[3] = 0;
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        pid = fork();
        std::stringstream buffer;
        if (!pid)
        {
            putenv((char *)"ciccio=cacca");
            putenv((char *)"REDIRECT_STATUS=true");
            putenv((char *)"SCRIPT_FILENAME=/var/www/");
            putenv((char *)"REQUEST_METHOD=POST");
            putenv((char *)"GATEWAY_INTERFACE=CGI/1.1");
            putenv((char *)"CONTENT_LENGTH=5");
            extern char **environ;
            env = environ;
            std::cout << env[10] << std::endl;
            dup2(fd, STDOUT_FILENO);
            execve("/Users/sgiovo/goinfre/._brew/bin/php-cgi", cmd, env);
            std::cout << "FATAL ERROR" << std::endl;
            std::cout << errno << std::endl;
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd);
            std::ifstream t(".__DamSuperCarino__", std::ifstream::in);
            buffer << t.rdbuf();
            t.close();
        }
//        for (int i = 0; i < 3; i++)
//            free(cmd[i]);
//        free(cmd);
        return buffer.str();
    }
};

