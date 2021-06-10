/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: forsili <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 09:47:56 by forsili           #+#    #+#             */
/*   Updated: 2021/06/10 10:40:41 by forsili          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../config/conf_parsing.hpp"
#include "Request.hpp"

#define DEFAULT_404 "<html><h1>ERROR 404 NOT FOUND</h1></html>"
#define DEFAULT_400 "<html><h1>ERROR 400 BAD REQUEST</h1></html>"

class Request;

class config;

class Response
{
	private:
		Request		request;
		config		conf;
		std::string	intestation;
		std::string	status;
		std::string	code;
		std::string	content_len;
		std::string	content_type;
		std::string	connection;
		std::string	body;
		
		std::string	read_path(std::string path, int code)
		{
			std::ifstream	myfile(path);
			std::string		buff;
			std::string		out;

			if (code == 200)
				this->intestation = this->request.http_version + "200 OK";
			else if (code == 404)
				this->intestation = this->request.http_version + "404 NOT FOUND";
			else if (code == 400)
				this->intestation = this->request.http_version + "400 BAD REQUEST";

			if (myfile.is_open())
			{
				while (getline(myfile, buff))
					out += buff + "\n";
				return out;
			}
			return "";
		}

		std::string	take_body(config c, Request r)
		{
			std::list<location>::iterator	it(c.locations.begin());
			int i = 0;
			
			if (r.error)
			{
				if (read_path(c.error_pages.getPath(400), 400) != "")
					return	read_path(c.error_pages.getPath(400), 400);
				return DEFAULT_400;
			}
			while (i < c.locations.size())
			{
				if (r.path.compare(it->path))
					return read_path(it->root);
				it++;
			}
			if (read_path(c.error_pages.getPath(404), 404) != "")
				return	read_path(c.error_pages.getPath(404), 404);
			return DEFAULT_404;
		}

	public:
		std::string	out;
		Response()
		{
			this->intestation = "";
			this->status = "";
			this->code ="";
			this->content_len = "";
			this->content_type = "";
			this->connection = "";
			config	tmp;
			this->conf = tmp;
			Request tmp_r;
			this->request = tmp_r;
		}

		Response(config c, Request r)
		{
			this->take_body(c, r);
			this->connection = r.connection;
			this->content_len = this->body.length();
			this->content_type = "text/html";

			this->out = this->intestation + "\n" + this->content_type + "\n" + this->content_len + "\n" +this->connection + "\n\n" + this->body + "\n";
		}
		//~Response();
};
