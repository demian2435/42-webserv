/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduregon <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 12:33:40 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/18 15:00:45 by aduregon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <list>
#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <fstream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

std::vector<std::string> split(std::string s, std::string del = " ")
	{
		std::vector<std::string> out;
	    int start = 0;
	    int end = s.find(del);
	    while (end != -1) {
	        out.push_back(s.substr(start, end - start));
	        start = end + del.size();
	        end = s.find(del, start);
	    }
	    out.push_back(s.substr(start, end - start));
		std::vector<std::string> ret;
		for (size_t i = 0; i < out.size(); i++)
			ret.push_back(out[i]);
		
		return ret;
	}

class ErrorPages
{
private:
	int         key[700];
	std::string path[700];
public:
	ErrorPages()
	{
		for (size_t i = 0; i < 700; i++)
		{
			key[i] = i;
			path[i] = "";
		}
	}

	~ErrorPages(){}
	
	void    insertPath(int code, std::string path)
	{
		this->path[code] = path;
	}

	std::string getPath(int code)
	{
		return this->path[code];
	}
};

class Location
{
public:
	std::string                     path;
    std::string                     root;
    std::string                     redirect;
    std::vector<std::string>        index;
	std::vector<std::string>		method;
    //std::list<std::string>          method;
    std::string                     cgi_path;
    std::string                     cgi_extension;
    bool                            autoindex;
    int								client_max_body_size;

public:
	Location()
	{
		this->path = "";
		this->root = "";
		this->index = std::vector<std::string>();
		this->method = std::vector<std::string>();
		this->cgi_extension = "";
		this->cgi_path = "";
		this->autoindex = false;
		this->client_max_body_size = -1;
	}

	Location(std::ifstream &file, std::string path)
	{
		std::string					buff;
		std::string					line;
		std::vector<std::string>	parse;
		int							i;
		
		this->client_max_body_size = -1;
		this->path = path;
		while (getline(file, buff))
		{
			i = 0;
			line = "";
			while (isspace(buff[i]))
				i++;
			while (buff[i] != ';' && buff[i] != '#' && buff[i])
			{
				line += buff[i];
				i++;
			}
			parse = split(line, " ");
			if (!parse[0].compare("root") && parse.size() == 2)
				this->root = parse[1];
			if (!parse[0].compare("cgi_path") && parse.size() == 2)
				this->cgi_path = parse[1];
			if (!parse[0].compare("redirect") && parse.size() == 2)
				this->redirect = parse[1];
			if (!parse[0].compare("cgi_extension") && parse.size() == 2)
				this->cgi_extension = parse[1];
			if (!parse[0].compare("method"))
			{
				for (size_t j = 1; j < parse.size(); j++)
					this->method.push_back(parse[j]);
			}
			if (!parse[0].compare("client_max_body_size") && parse.size() == 2)
			{
				this->client_max_body_size = std::stof(parse[1].c_str());
			}
			if (!parse[0].compare("autoindex") && parse.size() == 2)
			{
				if(!parse[1].compare("on"))
					this->autoindex = true;
				else
					this->autoindex = false;
			}
			if (!parse[0].compare("index") && parse.size() > 1)
			{
				for (size_t k = 1; k < parse.size(); k++)
					this->index.push_back(parse[k]);
			}
			if (!parse[0].compare("}"))
				break ;
			line.clear();
		}
	}

	~Location() {}
};

bool	compare_location(Location const &s1, Location const &s2)
{
	return (s1.path.size() > s2.path.size());
}

class Config_Server
{
public:
	std::string             	host;
    int			           		port;
    std::string             	name;
    ErrorPages              	error_pages;
	std::vector<Location>		location;
public:
	Config_Server(){}
	Config_Server(std::ifstream &file)
	{
		std::string					buff;
		std::string					line;
		std::vector<std::string>	parse;
		int			i;
		this->host = "127.0.0.1";
		//this->port = 80;
		while (getline(file, buff))
		{
			i = 0;
			while (isspace(buff[i]))
				i++;
			while (buff[i] != ';' && buff[i] != '#' && buff[i])
			{
				line += buff[i];
				i++;
			}
			parse = split(line, " ");
			if (!parse[0].compare("listen") && parse.size() == 3)
			{
				this->host = parse[1];
				this->port = std::stoi(parse[2].c_str());
			}
			if (!parse[0].compare("error_page") && parse.size() == 3)
				error_pages.insertPath(std::stoi(parse[1].c_str()), parse[2]);
			if (!parse[0].compare("server_name") && parse.size() == 2)
				this->name = parse[1];
			if (!parse[0].compare("location") && parse.size() == 3)
			{
				this->location.push_back(Location(file, parse[1]));
			}
			if (!parse[0].compare("}"))
			{
				std::sort(this->location.begin(), this->location.end(), compare_location);
				if (this->location.size() == 0)
				{
					std::cout << BOLDRED << "Error: no default location '/'\n" << RESET;
					exit(-1);
				}
				break ;
			}
			line = "";
			for (size_t y = 0; y < parse.size(); y++)
				parse[y].clear();
			
		}
	}
	
	~Config_Server() {}
};

class Config
{
public:
	std::vector<Config_Server>	server;
public:
	Config(std::string path = "./webserv.conf")
	{
		std::string					buff;
		std::string					line;
		std::vector<std::string>	parse;
		int			i;
		std::ifstream				file(path);
		
		if (!file.is_open())
		{
			std::cout << BOLDRED << "Error: can't open config file\n" << RESET;
			exit(-1);
		}
		while (getline(file, buff))
		{
			i = 0;
			while (isspace(buff[i]))
				i++;
			while (buff[i] != ';' && buff[i] != '#' && buff[i] != '{')
			{
				line += buff[i];
				i++;
			}
			parse = split(line, " ");
			if (!parse[0].compare("server"))
			{
				this->server.push_back(Config_Server(file));
			}
			line = "";
		}
	}

	~Config() {}
};

