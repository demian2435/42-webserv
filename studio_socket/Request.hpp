/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduregon <aduregon@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 11:59:50 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/09 13:18:42 by aduregon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <unistd.h>
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
		this->http_version = "1.1";
		this->host = "";
		this->connection = "keep-alive";
		this->referer = "";
		this->path = "/";
		this->body = "";
	}

	Request(char *r)
	{
		this->method = "GET";
		this->method_path = "/";
		this->http_version = "1.1";
		this->host = "";
		this->connection = "keep-alive";
		this->referer = "";
		this->path = "/";
		this->body = "";
		char *line;
		char **tmp;

		int i = 0;
		while (r[i])
		{
			line = NULL;
			while (r[i] != '\n')
			{
				line = ft_strjoin(line, r[i]);
				i++;
			}
			if (i == 0)
			{
				tmp = ft_split(line, ' ');
				if (matrix_len(tmp) == 3)
				{
					this->method = std::string(tmp[0]);
					this->method_path = std::string(tmp[1]);
					this->http_version = std::string(ft_strtrim(&tmp[2], "HTTP/", 1));
				}
				else
				{
					std::cout << "ERRORE\n";
				}
			}
			else
			{
				tmp = ft_split(line, ':');
				for (int k = 0; k < matrix_len(tmp); k++)
					tmp[k] = ft_strtrim(&tmp[k], " ", 1);
				if (!tmp)
					parse_body(r, i);
				else if (!strcmp(tmp[0], "Host") && strlen(tmp[0]) == strlen("Host"))
				{
					if (matrix_len(tmp) == 2)
						this->host = std::string(tmp[1]);
				}
				else if (!strcmp(tmp[0], "Connection") && strlen(tmp[0]) == strlen("Connection"))
				{
					if (matrix_len(tmp) == 2)
						this->connection = std::string(tmp[1]);
				}
				else if (!strcmp(tmp[0], "Referer") && strlen(tmp[0]) == strlen("Referer"))
				{
					if (matrix_len(tmp) == 2)
						this->referer = std::string(tmp[1]);
				}
			}
			if (r[i])
				i++;
		}
	}

	~Request() {}
};

