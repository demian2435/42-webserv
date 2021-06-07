/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_parsing.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: forsili <forsili@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 09:43:52 by forsili           #+#    #+#             */
/*   Updated: 2021/06/07 11:43:45 by forsili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <iostream>
#include <list>
#include <fstream>
#include <string.h>

#define MAX_SIZET 18446744073709599
#define MIN_INT -2147483648

class   location{
    private:

        std::string                     name;
        std::string                     root;
        std::list<std::string>          index;
        std::list<std::string>          method;
        std::string                     cgi_path;
        std::string                     cgi_extention;
        bool                            autoindex;
        size_t                          client_max_body_size;

    public:
        
        location(void)
        {
            this->name = "";
            this->root = "";
            this->cgi_path = "";
            this->cgi_extention = "";
            this->autoindex = false;
            this->client_max_body_size = MAX_SIZET;
        }

        ~location(){}

        void    setRoot(std::string input)
        {
            this->root = input;
        }

        void    setName(std::string input)
        {
            this->name = input;
        }

        void    setIndex(std::string input)
        {
            this->index.push_back(input);
        }

        void    setIndex(std::string *input, int size)
        {
            for (size_t i = 0; i < size; i++)
                this->index.push_back(input[i]);
        }

        void    setMethod(std::string input)
        {
            this->method.push_back(input);
        }

        void    setMethod(std::string *input, int size)
        {
            for (size_t i = 0; i < size; i++)
                this->method.push_back(input[i]);
        }

        void    setCgiExtension(std::string input)
        {
            this->cgi_extention = input;
        }

        void    setCgiPath(std::string input)
        {
            this->cgi_path = input;
        }

        void    setAutoindex(bool input)
        {
            this->autoindex = input;
        }

        void    setMaxBodySize(size_t input)
        {
            this->client_max_body_size = input;
        }

        std::string     getRoot()
        {
            return this->root;
        }

        std::string     getName()
        {
            return this->name;
        }

        std::list<std::string>  getIndex()
        {
            return this->index;
        }

        std::list<std::string>  getMethod()
        {
            return this->method;
        }

        std::string     getCgiPath()
        {
            return  this->cgi_path;
        }

        std::string getCgiExtension()
        {
            return this->cgi_extention;
        }

        bool        getAutoindex()
        {
            return this->autoindex;
        }

        size_t      getMaxBodySize()
        {
            return this->client_max_body_size;
        }

};

class   config{
    

    private:
        std::string             host;
        std::string             port;
        std::string             name;
        std::list<std::string>  error_pages;
        std::list<location>     locations;

    public:
        config()
        {
            std::string path = "./config/webserv.conf";
            std::string line;
            std::string buffer;
            std::ifstream myfile (path);
            if (myfile.is_open())
            {
                while ( getline (myfile,line) )
                    buffer.append(line + "\n");
                myfile.close();
                //this->parse(buffer);
            }
            else
                std::cerr << "Error: can't open config file\n";
            std::cout << buffer;
        }   
};