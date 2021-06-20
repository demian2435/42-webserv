/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CgiManager.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalori <dmalori@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 16:30:33 by sgiovo            #+#    #+#             */
/*   Updated: 2021/06/20 16:28:16 by dmalori          ###   ########.fr       */
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
#include "Server.hpp"
#include "Request.hpp"

#define PHP_GCI_PATH ("/Users/" + std::string(getenv("USER")) +  "/goinfre/.brew/bin/php-cgi")
class CgiManager
{
public:
    typedef std::pair<std::string, std::string>     Tpair;
    static Tpair      solve_all(std::string const &path, Request &req, std::string const &cgi_path, std::string const &cgi_extension = ".php")
    {
        if (!cgi_extension.compare(".php") || !cgi_extension.compare(".py"))
            return  solve_php(path, req, cgi_path, cgi_extension);
        else if (!cgi_extension.compare(".bla") && !req.method.compare("POST"))
                return (solve_php(path, req, cgi_path, cgi_extension));
        else if (!cgi_extension.compare(".bla"))
            return (solve_bla(path, req, cgi_path));
        return Tpair();
    }
    static Tpair      solve_php(std::string const &path, Request &req, std::string const &cgi_path, std::string const &cgi_extension)
    {
        if (!req.method.compare("POST"))
            return (solve_php_POST(path, req, cgi_path));
        else if (!req.method.compare("GET"))
            return (solve_php_GET(path, req, cgi_path, req.var ,cgi_extension));
        return Tpair();
    }

    static Tpair      solve_php_POST(std::string const & path, Request &req, std::string const & cgi_path)
    {
        pid_t pid;
        char **cmd = vecToCmd(req.var, cgi_path, path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        in_file(req);
        int in = open(".__in_file__", O_RDONLY);
        std::string result, head;
        std::stringstream sstr;
        std::string fPath("SCRIPT_FILENAME=" + path);
        std::string self = "http://" + req.host + req.path;
        std::string cookie = req.cookie;
        std::string cont_type = "CONTENT_TYPE=" + req.content_type;
        std::string method = "REQUEST_METHOD=" + req.method;
        std::string cont_len;
        sstr << req.body.length();
        cont_len = "CONTENT_LENGTH=" + sstr.str();
        size_t pos;
        while ((pos = self.find("\r")) != std::string::npos)
            self.erase(pos, 1);
        self = "PHP_SELF=" + self;
        while ((pos = cookie.find("; ")) != std::string::npos)
            cookie.replace(pos, 2, ";");
        cookie = "HTTP_COOKIE=" + cookie;
        std::stringstream buffer;
        pid = fork();
        if (!pid)
        {
            putenv((char *)("PATH_INFO=/"));
            putenv((char *)self.c_str());
            putenv((char *)cookie.c_str());
            putenv((char *)fPath.c_str());
            putenv((char *)cont_type.c_str());
            putenv((char *)method.c_str());
            putenv((char *)cont_len.c_str());
            putenv((char *)("SERVER_PROTOCOL=HTTP/1.1"));
            putenv((char *)("REDIRECT_STATUS=true"));
            putenv((char *)("GATEWAY_INTERFACE=CGI/1.1"));
            extern char **environ;
            dup2(fd, STDOUT_FILENO);
            dup2(in,STDIN_FILENO);
            close(in);
            execve(cgi_path.c_str(), cmd, environ);
            std::cout << "FATAL ERROR" << std::endl;
            std::cout << errno << std::endl;
            exit(1);
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd);
            std::ifstream t(".__DamSuperCarino__", std::ifstream::in);
            buffer << t.rdbuf();
            result = buffer.str();
            size_t pos = result.find("\r\n\r\n", 4);
            head = result.substr(0, pos);
            head += "\nCookie:" + req.cookie;
            result = result.substr(pos + 4);
            t.close();
        }
        for (size_t i = 0; cmd[i]; i++)
            free(cmd[i]);
        free(cmd);
        return Tpair(head, result);
    }
    static Tpair     solve_php_GET(std::string const & path, Request &req, std::string const &cgi_path, std::vector<std::string> const & vec, std::string const &cgi_extension)
    {
        pid_t pid;
        char **cmd;
        if (cgi_extension==".py")
            cmd = vecToCmdpy(vec, cgi_path, path);
        else
            cmd = vecToCmd(vec, cgi_path, path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        std::stringstream buffer;
        std::string self = req.path;
        std::string cookie = req.cookie;
        std::string result, head;
        std::string fPath("SCRIPT_FILENAME=" + path);
        size_t pos;

        while ((pos = self.find("\r")) != std::string::npos)
            self.erase(pos, 1);
        self = "PHP_SELF=" + self;
        while ((pos = cookie.find("; ")) != std::string::npos)
            cookie.replace(pos, 2, ";");
        cookie = "HTTP_COOKIE=" + cookie;
        //putenv((char *)fPath.c_str());
        pid = fork();
        if (!pid)
        {
            putenv((char *)cookie.c_str());
            putenv((char *)self.c_str());
            extern char **environ;
            dup2(fd, STDOUT_FILENO);
            execve(cmd[0], cmd, environ);
            std::cout << "FATAL ERROR" << std::endl;
            std::cout << errno << std::endl;
            exit(1);
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd);
            std::ifstream t(".__DamSuperCarino__", std::ifstream::in);
            buffer << t.rdbuf();
            result = buffer.str();
//            size_t pos = result.find(
//                    "X-Powered-By: PHP/8.0.7\nContent-type: text/html; charset=UTF-8", 63);
//            if (pos != std::string::npos)
//                result = result.substr(pos + 63);
            t.close();
        }
        for (size_t i = 0; cmd[i]; i++)
            free(cmd[i]);
        free(cmd);
        return Tpair(std::string(), result);
    }

    static Tpair  solve_bla(std::string const & path, Request &req, std::string const &cgi_path)
    {
        pid_t pid;
        char **cmd = vecToCmd(req.var, cgi_path, path);
        int fd = open(".__DamSuperCarino__", O_RDWR| O_CREAT | O_TRUNC , 0777);
        int in = open(path.c_str(), O_RDONLY);
        std::stringstream buffer;
        std::stringstream sstr;
        std::string fPath("SCRIPT_FILENAME=" + path);
        std::string self = "http://" + req.host + req.path;
        std::string cookie = req.cookie;
        std::string cont_type = "CONTENT_TYPE=" + req.content_type;
        std::string method = "REQUEST_METHOD=" + req.method;
        std::string cont_len;
        sstr << req.body.length();
        cont_len = "CONTENT_LENGTH=" + sstr.str();
        size_t pos;
        while ((pos = self.find("\r")) != std::string::npos)
            self.erase(pos, 1);
        self = "PHP_SELF=" + self;
        while ((pos = cookie.find("; ")) != std::string::npos)
            cookie.replace(pos, 2, ";");
        cookie = "HTTP_COOKIE=" + cookie;
        pid = fork();
        std::string result;
        if (!pid)
        {
            putenv((char *)("PATH_INFO=/"));
            putenv((char *)cookie.c_str());
            putenv((char *)fPath.c_str());
            if (!req.method.compare("POST"))
                putenv((char *)cont_type.c_str());
            putenv((char *)self.c_str());
            putenv((char *)method.c_str());
            putenv((char *)cont_len.c_str());
            putenv((char *)("SERVER_PROTOCOL=HTTP/1.1"));
            putenv((char *)("REDIRECT_STATUS=true"));
            putenv((char *)("GATEWAY_INTERFACE=CGI/1.1"));
            extern char **environ;
            dup2(fd, STDOUT_FILENO);
            dup2(in,STDIN_FILENO);
            close(in);
            execve(cgi_path.c_str(), cmd, environ);
            std::cout << "FATAL ERROR" << std::endl;
            std::cout << errno << std::endl;
            exit(1);
        }
        else
        {
            waitpid(pid, NULL, 0);
            close(fd);
            std::ifstream t(".__DamSuperCarino__", std::ifstream::in);
            buffer << t.rdbuf();
            result = buffer.str();
            size_t pos = result.find("\r\n\r\n", 4);
            result = result.substr(pos + 4);
            t.close();
        }
        for (size_t i = 0; cmd[i]; i++)
            free(cmd[i]);
        free(cmd);
        return Tpair(std::string(), result);
    }
    static char **vecToCmd(std::vector<std::string> const & vec, std::string const & cgi_path, std::string const & path)
    {
        int std_args = 2;
        char **cmd = (char **)malloc(sizeof(char *) * (vec.size() + std_args + 1));
        cmd[0] = strdup((char *)(cgi_path).c_str());
        cmd[1] = strdup(path.c_str());
        for(size_t i=0; i < vec.size(); i++)
            cmd[i+std_args] = strdup((vec[i].c_str()));
        cmd[vec.size() + std_args] = 0;
        return cmd;
    }
    static char **vecToCmdpy(std::vector<std::string> const & vec, std::string const & cgi_path, std::string const & path)
    {
        int std_args = 2;
        std::string tmp = "";
        char **cmd = (char **)malloc(sizeof(char *) * (vec.size() + std_args + 1));
        cmd[0] = strdup((char *)(cgi_path).c_str());
        // cmd[1] = strdup((char *)"-f");
        cmd[1] = strdup(path.c_str());
        for (size_t i=0; i < vec.size(); i++)
        {
            if (i > 0)
                tmp += "&";
            tmp += vec[i];
        }
        cmd[2] = strdup(tmp.c_str());
        cmd[3] = 0;
        return cmd;
    }
    static void in_file(Request &req)
    {
        std::ofstream out(".__in_file__", std::ofstream::out);
        out << req.body;
        out.close();
    }
};
