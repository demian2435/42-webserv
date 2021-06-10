/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduregon <aduregon@42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/09 11:59:50 by aduregon          #+#    #+#             */
/*   Updated: 2021/06/10 10:39:31 by aduregon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <stdlib.h>
#include "../config/conf_parsing.hpp"

class Request
{
private:
	std::string		method;
	std::string		method_path;
	std::string		http_version;
	std::string		a_im;
	std::string		accept;
	std::string		accept_charset;
	std::string		accept_encoding;
	std::string		accept_language;
	std::string		authorization;
	std::string		cache_control;
	std::string		content_encoding;
	int				content_length;
	std::string		content_type;
	std::string		cookie;
	std::string		date;
	std::string		expect;
	std::string		forwarded;
	std::string		from;
	std::string		http2_settings;
	std::string		if_match;
	std::string		if_modified_since;
	std::string		if_none_match;
	std::string		if_range;
	std::string		if_unmodified_since;
	std::string		max_forwards;
	std::string		origin;
	std::string		pragma;
	std::string		prefer;
	std::string		proxy_authorization;
	std::string		range;
	std::string		te;
	std::string		trailer;
	std::string		transfer_encoding;
	std::string		user_agent;
	std::string		upgrade;
	std::string		via;
	std::string		warning;
	std::string		host;
	std::string		connection;
	std::string		referer;
	std::string		path;
	std::string		body;
	bool			error;

public:
	Request(/* args */)
	{
		this->method = "";
		this->method_path = "/";
		this->http_version = "";
		this->a_im = "";
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
		this->date = "";
		this->expect = "";
		this->forwarded = "";
		this->from = "";
		this->http2_settings = "";
		this->if_match = "";
		this->if_modified_since = "";
		this->if_none_match = "";
		this->if_range = "";
		this->if_unmodified_since = "";
		this->max_forwards = "";
		this->origin = "";
		this->pragma = "";
		this->prefer = "";
		this->proxy_authorization = "";
		this->range = "";
		this->te = "";
		this->trailer = "";
		this->transfer_encoding = "";
		this->user_agent = "";
		this->upgrade = "";
		this->via = "";
		this->warning = "";
		this->host = "";
		this->connection = "keep-alive";
		this->referer = "";
		this->path = "";
		this->body = "";
		this->error = false;
	}

	Request(char *r)
	{
		this->method = "";
		this->method_path = "/";
		this->http_version = "";
		this->a_im = "";
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
		this->date = "";
		this->expect = "";
		this->forwarded = "";
		this->from = "";
		this->http2_settings = "";
		this->if_match = "";
		this->if_modified_since = "";
		this->if_none_match = "";
		this->if_range = "";
		this->if_unmodified_since = "";
		this->max_forwards = "";
		this->origin = "";
		this->pragma = "";
		this->prefer = "";
		this->proxy_authorization = "";
		this->range = "";
		this->te = "";
		this->trailer = "";
		this->transfer_encoding = "";
		this->user_agent = "";
		this->upgrade = "";
		this->via = "";
		this->warning = "";
		this->host = "";
		this->connection = "";
		this->referer = "";
		this->path = "";
		this->body = "";
		this->error = false;
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
				this->method_path.clear();
				while (str[i] != 32)
				{
					this->method_path += str[i];
					i++;
				}
				while (str[i] == 32)
					i++;
				while (str[i] != '\n')
				{
					this->http_version += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 5, "A-IM:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->a_im += str[i];
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
			else if (!(str.compare(i, 15, "Content-Length:")))
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
			else if (!(str.compare(i, 5, "Date:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->date += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 7, "Expect:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->expect += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 10, "Forwarded:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->forwarded += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 5, "From:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->from += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 15, "HTTP2-Settings:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->http2_settings += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 9, "If-Match:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->if_match += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 18, "If-Modified-Since:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->if_modified_since += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 18, "If-None-Match:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->if_none_match += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 9, "If-Range:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->if_range += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 20, "If-Unmodified-Since:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->if_unmodified_since += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 13, "Max-Forwards:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->max_forwards += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 7, "Origin:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->origin += str[i];
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
			else if (!(str.compare(i, 20, "Proxy-Authorization:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->proxy_authorization += str[i];
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
			else if (!(str.compare(i, 3, "TE:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->te += str[i];
					i++;
				}
			}
			else if (!(str.compare(i, 8, "Trailer:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->trailer += str[i];
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
			else if (!(str.compare(i, 4, "Via:")))
			{
				while (str[i] != 32)
					i++;
				i++;
				while (str[i] != '\n')
				{
					this->via += str[i];
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
		this->check_request();
	}

	~Request() {}

	void	check_request()
	{
		if (this->method.compare("GET") && this->method.compare("POST") && this->method.compare("DELETE"))
		{
			this->error = true;
			return ;
		}
		if (!(this->method_path.compare("")))
		{
			this->error = true;
			return ;
		}
		if (!(this->http_version.compare("")))
		{
			this->error = true;
			return ;
		}
		if (!(this->connection.compare("")))
		{
			this->error = true;
			return ;
		}
		if (!(this->host.compare("")))
		{
			this->error = true;
			return ;
		}
		if (this->referer.compare(""))
		{
			int j = 7 + this->host.length();
			while (this->referer[j])
			{
				this->path += this->referer[j];
				j++;
			}
		}
		else
			this->path = this->method_path;
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
	if (this->a_im.compare(""))
		std::cout << this->a_im << std::endl;
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
	// std::string		date;
	if (this->date.compare(""))
		std::cout << this->date << std::endl;
	// std::string		expect;
	if (this->expect.compare(""))
		std::cout << this->expect << std::endl;
	// std::string		forwarded;
	if (this->forwarded.compare(""))
		std::cout << this->forwarded << std::endl;
	// std::string		from;
	if (this->from.compare(""))
		std::cout << this->from << std::endl;
	// std::string		http2_settings;
	if (this->http2_settings.compare(""))
		std::cout << this->http2_settings << std::endl;
	// std::string		if_match;
	if (this->if_match.compare(""))
		std::cout << this->if_match << std::endl;
	// std::string		if_modified_since;
	if (this->if_modified_since.compare(""))
		std::cout << this->if_modified_since << std::endl;
	// std::string		if_none_match;
	if (this->if_none_match.compare(""))
		std::cout << this->if_none_match << std::endl;
	// std::string		if_range;
	if (this->if_range.compare(""))
		std::cout << this->if_range << std::endl;
	// std::string		if_unmodified_since;
	if (this->if_unmodified_since.compare(""))
		std::cout << this->if_unmodified_since << std::endl;
	// std::string		max_forwards;
	if (this->max_forwards.compare(""))
		std::cout << this->max_forwards << std::endl;
	// std::string		origin;
	if (this->origin.compare(""))
		std::cout << this->origin << std::endl;
	// std::string		pragma;
	if (this->pragma.compare(""))
		std::cout << this->pragma << std::endl;
	// std::string		prefer;
	if (this->prefer.compare(""))
		std::cout << this->prefer << std::endl;
	// std::string		proxy_authorization;
	if (this->proxy_authorization.compare(""))
		std::cout << this->proxy_authorization << std::endl;
	// std::string		range;
	if (this->range.compare(""))
		std::cout << this->range << std::endl;
	// std::string		te;
	if (this->te.compare(""))
		std::cout << this->te << std::endl;
	// std::string		trailer;
	if (this->trailer.compare(""))
		std::cout << this->trailer << std::endl;
	// std::string		transfer_encoding;
	if (this->transfer_encoding.compare(""))
		std::cout << this->transfer_encoding << std::endl;
	// std::string		user_agent;
	if (this->user_agent.compare(""))
		std::cout << this->user_agent << std::endl;
	// std::string		upgrade;
	if (this->upgrade.compare(""))
		std::cout << this->upgrade << std::endl;
	// std::string		via;
	if (this->via.compare(""))
		std::cout << this->via << std::endl;
	// std::string		warning;
	if (this->warning.compare(""))
		std::cout << this->warning << std::endl;
	// std::string		host;
	if (this->host.compare(""))
		std::cout << this->host << std::endl;
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
	}
};

