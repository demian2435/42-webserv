/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalori <dmalori@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 11:59:50 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/17 16:40:38 by dmalori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

class Request
{
public:
	std::string					method;
	std::string					method_path;
	std::string					http_version;
	std::string					accept;
	std::string					accept_charset;
	std::string					accept_encoding;
	std::string					accept_language;
	std::string					authorization;
	std::string					cache_control;
	std::string					content_encoding;
	int							content_length;
	std::string					content_type;
	std::string					cookie;
	std::string					pragma;
	std::string					prefer;
	std::string					range;
	std::string					transfer_encoding;
	std::string					user_agent;
	std::string					upgrade;
	std::string					warning;
	std::string					host;
	std::string					host_ip;
	int							host_port;
	std::string					connection;
	std::string					referer;
	std::string					path;
	std::string					body;
	std::vector<std::string>	var;
	bool						upload;
	std::string					buff;

public:
	Request(/* args */)
	{
		this->method = "";
		this->method_path = "/";
		this->http_version = "";
		this->accept = "";
		this->accept_charset = "";
		this->accept_encoding = "";
		this->accept_language = "";
		this->authorization = "";
		this->cache_control = "";
		this->content_encoding = "";
		this->content_length = 0;
		this->content_type = "";
		this->cookie = "";
		this->pragma = "";
		this->prefer = "";
		this->range = "";
		this->transfer_encoding = "";
		this->user_agent = "";
		this->upgrade = "";
		this->warning = "";
		this->host = "";
		this->host_ip = "";
		this->host_port = 0;
		this->connection = "keep-alive";
		this->referer = "";
		this->path = "";
		this->body = "";
		this->upload = false;
		this->buff = "";
	}

	Request(std::string str)
	{
		this->content_length = 0;
		this->host_port = 0;
		this->upload = false;
		this->buff = str;

		int i = 0;
		while (str[i])
		{
			if (i == 0)
			{
				while (str[i] && str[i] != 32)
				{
					this->method +=  str[i];
					i++;
				}
				//std::cout << this->method << std::endl;
				i++;
				this->method_path.clear();
				while (str[i] && str[i] != 32)
				{
					this->method_path += str[i];
					i++;
				}
				//std::cout << "PATH: " << this->method_path << std::endl;
				while (str[i] && str[i] == 32)
					i++;
				while (str[i] && str[i] != '\n')
				{
					this->http_version += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 7, "Accept:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->accept += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 15, "Accept-Charset:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->accept_charset += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 16, "Accept-Encoding:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->accept_encoding += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 16, "Accept-Language:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->accept_language += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 14, "Authorization:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->authorization += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 14, "Cache-Control:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->cache_control += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 17, "Content-Encoding:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->content_encoding += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 15, "content-length:")))
			{
				std::string tmp;
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					tmp += str[i];
					i++;
				}
				this->content_length = std::stoi(tmp);
			}
			else if (!(str.compare(i, 13, "Content-Type:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->content_type += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 7, "Cookie:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->cookie += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 7, "Pragma:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->pragma += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 7, "Prefer:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->prefer += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 6, "Range:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->range += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 18, "Transfer-Encoding:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->transfer_encoding += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 11, "User-Agent:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->user_agent += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 8, "Upgrade:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->upgrade += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 8, "Warning:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->warning += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 5, "Host:")))
			{
				this->host.clear();
				while (str[i] != 32)
					i++;
				i++;
				int j = i;
				while (str[i] != '\n')
				{
					this->host += str[i];
					i++;
				}
				while (str[j] && str[j] != ':' && str[j] != '\n')
				{
					this->host_ip += str[j];
					j++;
				}
				if (!this->host_ip.compare("localhost"))
					this->host_ip = "127.0.0.1";
				if (str[j] == ':')
				{
					j++;
					std::string tmp;
					while (str[j] && str[j] != '\n')
					{
						tmp += str[j];
						j++;
					}
					this->host_port = std::stoi(tmp);				
				}
			}
			else if ( !(str.compare(i, 11, "Connection:")))
			{
				this->connection.clear();
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->connection += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 8, "Referer:")))
			{
				this->referer.clear();
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->referer += str[i];
					i++;
				}
			}
			else if (str[i] == '\n' && str[i + 2] && str[i + 2] == '\n')
			{
				i += 3;
				break;
			}
			else
				i++;
		}
		while (str[i])
		{
			this->body += str[i];
			i++;
		}
		this->parse_request();
		//this->print_request();
	}

	~Request() {}

	bool	is_valid()
	{
		if (this->method.compare("GET") && this->method.compare("POST") &&
			this->method.compare("DELETE") && this->method.compare("PUT") &&
			this->method.compare("HEAD"))
		{
			puts("UNO");
			return false;
		}
		if (!(this->method_path.compare("")))
		{
			puts("DUE");
			return false;
		}
		if (!(this->http_version.compare("HTTP/1.1")))
		{
			puts("TRE");
			return false;
		}
		if (!(this->host.compare("")))
		{
			puts("CINQUE");
			return false;
		}
		return true;
	}

	std::vector<std::string> tokenize(std::string s, std::string del = " ")
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
		return out;
	}

	bool	contain_var(std::string str)
	{
		for (size_t i = 0; i < str.length(); i++)
			if (str[i] == '?')
				return true;
		return false;
	}

	bool	is_ready() const
	{
		if (!(this->method.compare("PUT")) || !(this->method.compare("POST")))
		{
			if (this->content_length > 0)
			{
				std::cout << this->body.length() << " " << this->content_length << " \n";
				if (this->body.length() != (size_t)this->content_length)
					return false;
			}
			else if (!(this->transfer_encoding.compare(0, 7, "chunked")))
			{
				if (std::strcmp(this->buff.substr(this->buff.length() - 5).c_str(), "0\r\n\r\n"))
					return false;
			}
		}
		else
		{
			if (std::strcmp(this->buff.substr(this->buff.length() - 4).c_str(), "\r\n\r\n"))
				return false;
		}
		return true;
	}
	
	void	parse_request()
	{
		if (!(this->transfer_encoding.compare(0, 7, "chunked")) && this->transfer_encoding.size() >= 8)
			this->transfer_encoding.erase(7, this->transfer_encoding.size() - 7);
		
		if (!(this->method.compare("PUT")) || !(this->method.compare("POST")))
		{
			if ((this->content_length != 0 && this->content_type.compare("")) ||
				!(this->transfer_encoding.compare(0, 7, "chunked")))
				this->upload = true;
		}
		if (this->method_path.compare(""))
		{
			if (this->contain_var(this->method_path))
			{
				std::vector<std::string> tmp = tokenize(this->method_path, "?");
				this->method_path.clear();
				this->method_path = tmp[0];
				tmp = tokenize(tmp[1], "&");
				for (size_t i = 0; i < tmp.size(); i++)
					this->var.push_back(tmp[i]);
			}
			if (this->method_path.length() > 1 && this->method_path[this->method_path.length() - 1] == '/')
				this->path = this->method_path.substr(0, this->method_path.length() - 1);
			else
				this->path = this->method_path;
		}
	}

	void	print_request()
	{
		//	std::string		method;
		if (this->method.compare(""))
			std::cout << this->method << " ";
		// std::string		method_path;
		if (this->method_path.compare(""))
			std::cout << this->method_path << " ";
		// std::string		http_version;
		if (this->http_version.compare(""))
			std::cout << this->http_version << std::endl;
		// std::string		a_im;
		// std::string		accept;
		if (this->accept.compare(""))
			std::cout << this->accept << std::endl;
		// std::string		accept_charset;
		if (this->accept_charset.compare(""))
			std::cout << this->accept_charset << std::endl;
		// std::string		accept_encoding;
		if (this->accept_encoding.compare(""))
			std::cout << this->accept_encoding << std::endl;
		// std::string		accept_language;
		if (this->accept_language.compare(""))
			std::cout << this->accept_language << std::endl;
		// std::string		authorization;
		if (this->authorization.compare(""))
			std::cout << this->authorization << std::endl;
		// std::string		cache_control;
		if (this->cache_control.compare(""))
			std::cout << this->cache_control << std::endl;
		// std::string		content_encoding;
		if (this->content_encoding.compare(""))
			std::cout << this->content_encoding << std::endl;
		// int				content_length;
		if (this->content_length != 0)
			std::cout << this->content_length << std::endl;
		// std::string		content_type;
		if (this->content_type.compare(""))
			std::cout << this->content_type << std::endl;
		// std::string		cookie;
		if (this->cookie.compare(""))
			std::cout << this->cookie << std::endl;
		// std::string		pragma;
		if (this->pragma.compare(""))
			std::cout << this->pragma << std::endl;
		// std::string		prefer;
		if (this->prefer.compare(""))
			std::cout << this->prefer << std::endl;
		// std::string		range;
		if (this->range.compare(""))
			std::cout << this->range << std::endl;
		// std::string		transfer_encoding;
		if (this->transfer_encoding.compare(""))
			std::cout << this->transfer_encoding << std::endl;
		// std::string		user_agent;
		if (this->user_agent.compare(""))
			std::cout << this->user_agent << std::endl;
		// std::string		upgrade;
		if (this->upgrade.compare(""))
			std::cout << this->upgrade << std::endl;
		// std::string		warning;
		if (this->warning.compare(""))
			std::cout << this->warning << std::endl;
		// std::string		host;
		if (this->host.compare(""))
		{
			std::cout << this->host << std::endl;
			std::cout << RED << this->host_ip << std::endl;
			std::cout << this->host_port << RESET << std::endl;
		}
		// std::string		connection;
		if (this->connection.compare(""))
			std::cout << this->connection << std::endl;
		// std::string		referer;
		if (this->referer.compare(""))
			std::cout << this->referer << std::endl;
		// std::string		path;
		if (this->path.compare(""))
			std::cout << this->path << std::endl;
		// std::string		body;
		if (this->body.compare(""))
			std::cout << std::endl << this->body << std::endl;
		if (this->var.size() != 0)
		{
			for (size_t i = 0; i < this->var.size(); i++)
				std::cout << this->var[i] << " ";
			std::cout << std::endl;
		}
	}
	
};

