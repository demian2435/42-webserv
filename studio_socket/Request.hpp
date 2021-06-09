/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduregon <aduregon@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 11:59:50 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/09 17:33:22 by aduregon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include "../config/conf_parsing.hpp"

char		*ft_strjoin(char *s1, char s2)
{
	char *tmp;

	if (!s1)
	{
		tmp = (char *)calloc(2, sizeof(char));
		tmp[0] = s2;
		return (tmp);
	}
	int		s1_len = strlen(s1);
	tmp = (char *)calloc((s1_len + 2), sizeof(char));
	int i = 0;
	while (s1[i])
	{
		tmp[i] = s1[i];
		i++;
	}
	tmp[i] = s2;
	//free(s1);
	return (tmp);
}

class Request
{
private:
	std::string		method;
	std::string		method_path;
	std::string		http_version;
	std::string		host;
	std::string		connection;
	std::string		referer;
	std::string		path;
	std::string		body;

	void	parse_body(char *r, int &i)
	{
		char *tmp = NULL;
		while (r[i])
		{
			tmp = ft_strjoin(tmp, r[i]);
			i++;
			
		}
		this->body = std::string(tmp);
	}

public:
	Request(/* args */)
	{
		this->method = "GET";
		this->method_path = "/";
		this->http_version = "HTTP/1.1";
		this->host = "";
		this->connection = "keep-alive";
		this->referer = "";
		this->path = "/";
		this->body = "";
	}

	Request(char *r)
	{
		this->method = "GET";
		this->method_path = "";
		this->http_version = "";
		this->host = "";
		this->connection = "keep-alive";
		this->referer = "";
		this->path = "/";
		this->body = "";
		std::string str = r;

		int i = 0;
		while (str[i])
		{
			if (i == 0)
			{
				if (!(str.compare(i, 3, "GET")))
				{
					this->method = "GET";
					i += 4;
				}
				else if (!(str.compare(i, 4, "POST")))
				{
					this->method = "POST";
					i += 5;
				}
				else if (!(str.compare(i, 6, "DELETE")))
				{
					this->method = "DELETE";
					i += 7;
				}
				else
				{
					std::cout << "\033[33m" << "Invalid method" << "\033[0m" << std::endl;
				}
				while (str[i] != 32)
				{
					this->method_path += str[i];
					i++;
				}
				while (str[i] != '\n')
				{
					this->http_version += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 5, "Host:")))
			{
				this->host.clear();
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->host += str[i];
					i++;
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
			else
			{
				if (str[i] == '\n' && str[i + 1] && str[i + 1] == '\n')
				{
					i += 2;
					break;
				}
				i++;
			}
		}
		while (str[i])
		{
			this->body += str[i];
			i++;
		}
		std::cout << this->method << " ";
		std::cout << this->method_path << " ";
		std::cout << this->http_version << std::endl;
		std::cout << this->host << std::endl;
		std::cout << this->connection << std::endl;
		std::cout << this->referer << std::endl;
		std::cout << this->body << std::endl;
	}

	~Request() {}
};

