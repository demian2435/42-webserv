#pragma once

#include "Config.hpp"
#include "studio_socket/Request.hpp"
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

		std::string	take_body(Server_Config c, Request r)
		{
			for (size_t i = 0; i < c.locations.size(); i++)
			{
				if (c)
			}
			
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

		Response(Server_Config c, Request r)
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