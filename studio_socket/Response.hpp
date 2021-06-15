#pragma once

#include "Config.hpp"
#include "Request.hpp"
#include <dirent.h>
#include <sstream>

#define DEFAULT_404 "<html><h1>ERROR 404 NOT FOUND</h1></html>"
#define DEFAULT_400 "<html><h1>ERROR 400 BAD REQUEST</h1></html>"

class Request;

class Config_Server;

class Response
{
	private:
		Config_Server		conf;
		Request		request;
		std::string	intestation;
		std::string	status;
		std::string	code;
		std::string	content_len;
		std::string	content_type;
		std::string	connection;
		std::string	body;
		
		int	find_path(std::string path, Config_Server c)
		{
			for (size_t i = 0; i < c.location.size(); i++)
			{
				std::cout << GREEN << "<" << c.location[i].path << ">" << std::endl;
				if (!c.location[i].path.compare(path))
					return i;
			}
			return -1;
		}

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

		std::string	take_body(Config_Server c, Request r)
		{
			int	start = this->find_path("/", c);
			std::string	subpath = "";
			for (size_t i = 0; i < c.location.size(); i++)
			{
				std::cout << c.location[i].path << " ------- " << r.path << std::endl;
				for (size_t k = 0; k < c.location[i].index.size(); k++)
				{
					if (r.path.find(c.location[i].path) == 0 && r.path.size() == c.location[i].path.size() && read_path(c.location[i].root + c.location[i].index[k], 200) != "")
						return read_path(c.location[i].root + c.location[i].index[k], 200);
				}
				if (r.path.find(c.location[i].path) == 0)
				{
					subpath = r.path.substr(c.location[i].path.length(), r.path.size() - c.location[i].path.length());
					if (read_path(subpath, 200) != "")
						return read_path(subpath, 200);
				}
				std::cout << i << std::endl;
				std::cout << c.location.size() << std::endl;
			}
			
			subpath =c.location[start].root + r.path;
			std::cout << c.location[start].path << "  -0--" << std::endl;
			
			if (read_path(subpath, 200) != "")
				return read_path(subpath, 200);
			if (read_path(c.error_pages.getPath(404), 404) != "")
				return	read_path(c.error_pages.getPath(404), 404);
			return DEFAULT_404;

		}

		void generate_autoindex(Config_Server c, Request r)
		{
			struct dirent *entry;
			std::string tmp;
			std::string directory;
			// posso passarmi direttamente start da take_body?
			int	start;
			std::cout << "PPP  <" << r.path << ">" << std::endl;
			if ((start = this->find_path(r.path, c)) == -1)
			{
				start = this->find_path("/", c);
				directory = c.location[start].root + r.path;
			}
			else
				directory = c.location[start].root;

			this->body = "<!DOCTYPE html><body><h1>Index of " + r.path + "</h1>";
			// std::cout << RED << c.name << RESET << std::endl;
			DIR *dir = opendir(directory.c_str());
			// questo andrebbe controllato prima
			if (dir == NULL) {
				this->body = DEFAULT_404;
				read_path(r.path.c_str(), 404);
				return;
			}
			while ((entry = readdir(dir)) != NULL) {
				// nasconde file
				if(entry->d_name[0] == '.')
					continue ;
				tmp = entry->d_name;
				this->body += "<li><a href=\"" + tmp + "\">" + tmp;
				// mette "/" dopo cartelle
				if (entry->d_type == DT_DIR)
					this->body += "/";
				this->body += "</a></li>";
				std::cout << RED << tmp << " : " << (entry->d_type == DT_DIR) << std::endl;
			}
			this->body += "</body></html>";
    		closedir(dir);
		}

	public:
		std::string	out;

		Response(Config_Server c, Request r) : conf(c)
		{
			this->request = r;
			this->body = take_body(c, r);
			//generate_autoindex(c, r);
			this->connection =  "Connection: " + r.connection;
			this->content_len = "Content-Length: " + std::to_string(this->body.length());
			this->content_type = "Content-Type: text/html";

			this->out = this->intestation + "\n" + this->content_type + "\n" + this->content_len + "\n" +this->connection + "\n\n" + this->body + "\n";
		}
		//~Response();
};
