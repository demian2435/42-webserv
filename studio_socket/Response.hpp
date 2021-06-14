/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalori <dmalori@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 09:47:56 by forsili           #+#    #+#             */
/*   Updated: 2021/06/14 16:00:38 by dmalori          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "conf_parsing.hpp"
#include "Request.hpp"
#include <sstream>

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
			//std::cout << path;
			std::cout << YELLOW << path << RESET << std::endl;
			std::ifstream	myfile(path);
			std::string		buff;
			std::string		out;

			if (code == 200)
				this->intestation = "HTTP/1.1 200 OK";
			else if (code == 404)
				this->intestation = "HTTP/1.1 404 NOT FOUND";
			else if (code == 400)
				this->intestation = "HTTP/1.1 400 BAD REQUEST";

			if (myfile.is_open())
			{
				while (getline(myfile, buff))
					out += buff + "\n";
				return out;
			}
			return "";
		}

		std::list<std::string>	listify_path(std::string path)
		{
			char **matrix = ft_split(path.c_str(), '/');
			std::list<std::string>	out;
			int i = 0;

			while (matrix[i])
			{
				out.push_back("/" + std::string(matrix[i]));
				i++;
			}
			if (out.size() == 0)
				out.push_back("/");
			free_matrix(matrix);
			return out;
		}

		std::string	take_body(config c, Request r)
		{
			std::list<location>::iterator	it(c.locations.begin());
			std::list<std::string>::iterator	index;
			size_t i = 0;
			
			if (r.error)
			{
				if (read_path(c.error_pages.getPath(400), 400) != "")
					return	read_path(c.error_pages.getPath(400), 400);
				return DEFAULT_400;
			}
			while (i < c.locations.size())
			{
				index = it->index.begin();
				//std::cout << r.path << " vs " << it->path << "X" <<  std::endl;
				std::list<std::string>				path_list = listify_path(r.path);
				std::list<std::string>::iterator	path_it(path_list.begin());
				std::string							final_path = "";
				std::cout << "######################### " << r.path << std::endl;
				if (!r.path.compare(it->path) && path_list.size() == 1)
				{
					for (size_t k = 0; k < it->index.size(); k++)
					{
						std::cout << it->root + *index <<std::endl;
						if (read_path(it->root + *index, 200) != "")
							return read_path(it->root + *index, 200);
						index++;
					}
					if (read_path(it->root, 200) != "")
							return read_path(it->root, 200);
				}
				else if (!path_it->compare(it->path) && path_list.size() > 1)
				{
					for (size_t k = 0; k < path_list.size(); k++)
					{
						if (i == 0)
							final_path += it->root;
						else
							final_path += *path_it;
						path_it++;
					}
					std::cout << "######################### " << final_path << std::endl;
					if (read_path(final_path, 200) != "")
							return read_path(final_path, 200);
				}
				it++;
				i++;
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
			this->conf = c;
			this->request = r;
			this->body = take_body(c, r);
			this->connection =  "Connection: " + r.connection;
			this->content_len = "Content-Length: " + std::to_string(this->body.length());
			this->content_type = "Content-Type: text/html";

			this->out = this->intestation + "\n" + this->content_type + "\n" + this->content_len + "\n" +this->connection + "\n\n" + this->body + "\n";
		}
		//~Response();
};

