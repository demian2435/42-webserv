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
#include <vector>

class CgiManager
{
private:
    char        **env;
    std::string user;
public:
    CgiManager(void):user(std::string(getenv("USER"))){}
    ~CgiManager(){}
    std::string     solve_php_POST(std::string path, std::string body, Request req)
    {
        pid_t pid;
        char **cmd = vecToCmd(std::vector<std::string>(), path);
        int spipe[2];
        pipe(spipe);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        pid = fork();
        std::stringstream buffer;
        if (!pid)
        {
            std::string fPath("SCRIPT_FILENAME=/Users/sgiovo/CLionProjects/42-webserv/studio_socket/");
            fPath += path;
            putenv((char *)"REDIRECT_STATUS=true");
            putenv((char *)fPath.c_str());
            putenv((char *)"REQUEST_METHOD=POST");
            putenv((char *)"GATEWAY_INTERFACE=CGI/1.1");
            putenv((char *)"CONTENT_TYPE=application/x-www-form-urlencoded");
            putenv((char *)"CONTENT_LENGTH=5000");
            extern char **environ;
            env = environ;
            dup2(fd, STDOUT_FILENO);
            dup2(spipe[0],STDIN_FILENO);
            close(spipe[0]);
            close(spipe[1]);
            execve("/Users/sgiovo/goinfre/._brew/bin/php-cgi", cmd, env);
            std::cout << "FATAL ERROR" << std::endl;
            std::cout << errno << std::endl;
        }
        else
        {
            close(spipe[0]);
            write(spipe[1],"name=BiscottodiAndrea&ciao=cocco", strlen("name=BiscottodiAndrea&ciao=cocco"));
            close(spipe[1]);
            waitpid(pid, NULL, 0);
            close(fd);
            std::ifstream t(".__DamSuperCarino__", std::ifstream::in);
            buffer << t.rdbuf();
            t.close();
        }
        return buffer.str();
    }
    std::string     solve_php_GET(std::string path, std::vector<std::string> vec)
    {
        pid_t pid;
        std::cout << user << std::endl;
        //gestire args qui sopra
        char **cmd = vecToCmd(vec,path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        pid = fork();
        std::stringstream buffer;
        if (!pid)
        {
            std::string fPath("SCRIPT_FILENAME=/Users/sgiovo/CLionProjects/42-webserv/studio_socket/");
            fPath += path;
            extern char **environ;
            env = environ;
            dup2(fd, STDOUT_FILENO);
            execve(cmd[0], cmd, env);
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
        return buffer.str();
    }
    char **vecToCmd(std::vector<std::string> const & vec, std::string path)
    {
        char **cmd = (char **)malloc(sizeof(char *) * (vec.size() + 3));
        cmd[0] = strdup((char *)("/Users/" + user +  "/goinfre/._brew/bin/php-cgi").c_str());
        cmd[1] = strdup(path.c_str());
        for(size_t i=0; i < vec.size(); i++)
        {

        cmd[i+2] = strdup((vec[i].c_str()));
        }
        cmd[vec.size() + 2] = 0;
        return cmd;
    }

};

