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
#include "../../../Desktop/studio_socket/Server.hpp"
#include "../../../Desktop/studio_socket/Request.hpp"

#define PHP_GCI_PATH ("/Users/" + std::string(getenv("USER")) +  "/goinfre/._brew/bin/php-cgi")
class CgiManager
{
public:
    static std::string      solve_all(Request &req, std::string const &cgi_path, std::string const &cgi_extension = ".php")
    {
        if (!cgi_extension.compare(".php"))
               return  solve_php(req, cgi_path);
        if (!cgi_extension.compare(".bla"))
                return (solve_bla(req, cgi_path));
        return "";
    }
    static std::string      solve_php(Request &req, std::string const &cgi_path)
    {
        if (!req.method.compare("POST"))
            return (solve_php_POST(req, cgi_path));
        else if (!req.method.compare("GET"))
            return (solve_php_GET(req, cgi_path, req.var));
        return "";
    }
    static std::string      solve_php_POST(Request &req, std::string const & cgi_path)
    {
        pid_t pid;
        char **cmd = vecToCmd(req.var, cgi_path, req.path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        in_file(req);
        int in = open(".__in_post__", O_RDONLY);
        pid = fork();
        std::stringstream buffer;
        if (!pid)
        {
            std::stringstream sstr;
            sstr << req.body.length();
            std::string fPath("SCRIPT_FILENAME=");
            fPath += req.path;
            putenv((char *)("REDIRECT_STATUS=true"));
            putenv((char *)fPath.c_str());
            putenv((char *)"REQUEST_METHOD=POST");
            putenv((char *)("GATEWAY_INTERFACE=CGI/1.1"));
            putenv((char *)("CONTENT_TYPE=" + req.content_type).c_str());
            putenv((char *)("CONTENT_LENGTH=" + sstr.str()).c_str());
            extern char **environ;
            dup2(fd, STDOUT_FILENO);
            dup2(in,STDIN_FILENO);
            close(in);
            execve(cgi_path.c_str(), cmd, environ);
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
        for (size_t i = 0; cmd[i]; i++)
            free(cmd[i]);
        free(cmd);
        return buffer.str();
    }
    static std::string     solve_php_GET(Request &req, std::string const &cgi_path, std::vector<std::string> const & vec)
    {
        pid_t pid;
        char **cmd = vecToCmd(vec, cgi_path, req.path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        std::stringstream buffer;
        pid = fork();
        if (!pid)
        {
            extern char **environ;
            dup2(fd, STDOUT_FILENO);
            execve(cmd[0], cmd, environ);
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
        for (size_t i = 0; cmd[i]; i++)
            free(cmd[i]);
        free(cmd);
        return buffer.str();
    }

    static std::string  solve_bla(Request &req, std::string const &cgi_path)
    {
        pid_t pid;
        char **cmd = vecToCmd(req.var, cgi_path, req.path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        int in = open(req.path.c_str(), O_RDONLY);
        pid = fork();
        std::stringstream buffer;
        if (!pid)
        {
            std::stringstream sstr, len;
            std::ifstream in_file(req.path);
            sstr << in_file.rdbuf();
            len << sstr.str().length();
            putenv((char *)("PATH_INFO=/"));
            putenv((char *)("SERVER_PROTOCOL=HTTP/1.1"));
            putenv((char *)("REQUEST_METHOD=" + req.method).c_str());
            putenv((char *)("GATEWAY_INTERFACE=CGI/1.1"));
            putenv((char *)("CONTENT_TYPE=" + req.content_type).c_str());
            putenv((char *)("CONTENT_LENGTH=" + len.str()).c_str());
            extern char **environ;
            dup2(fd, STDOUT_FILENO);
            dup2(in,STDIN_FILENO);
            close(in);
            execve(cgi_path.c_str(), cmd, environ);
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
        for (size_t i = 0; cmd[i]; i++)
            free(cmd[i]);
        free(cmd);
        return buffer.str();
    }
    static char **vecToCmd(std::vector<std::string> const & vec, std::string const & cgi_path, std::string const & path)
    {
        int std_args = 3;
        char **cmd = (char **)malloc(sizeof(char *) * (vec.size() + std_args + 1));
        cmd[0] = strdup((char *)(cgi_path).c_str());
        cmd[1] = strdup((char *)"-f");
        cmd[2] = strdup(path.c_str());
        for(size_t i=0; i < vec.size(); i++)
            cmd[i+std_args] = strdup((vec[i].c_str()));
        cmd[vec.size() + std_args] = 0;
        return cmd;
    }
    static void in_file(Request &req)
    {
        std::ofstream out(".__in_file__", std::ofstream::out);
        out << req.body;
        out.close();
    }
};

