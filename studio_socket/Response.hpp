#pragma once

#include "Config.hpp"
#include "Request.hpp"
#include "CgiManager.hpp"
#include <dirent.h>
#include <sstream>
#include <ctime>
#include <unistd.h>

#define DEFAULT_404 "<html><h1>ERROR 404 NOT FOUND</h1></html>\n"
#define DEFAULT_401 "<html><h1>ERROR 401 UNAUTORIZED</h1></html>\n"
#define DEFAULT_400 "<html><h1>ERROR 400 BAD REQUEST</h1></html>\n"
#define DEFAULT_405 "<html><h1>ERROR 405 METHOD NOT ALLOWED</h1></html>\n"
#define PHP 1
#define BLA 2
//DEBUG
#define debug false

class Request;

class Config_Server;

class Response
{
	private:
		Config_Server		conf;
		Request				request;
		std::string			intestation;
		std::string			status;
		std::string			content_len;
		std::string			content_type;
		std::string			connection;
		std::string			body;
		bool				oversize;
		bool				redirection;

		std::string extension(std::string path)
		{
			std::string tmp = "";
		
			if (path.length() < 5)
				return "";
			for (size_t i = path.length() - 1; i > 0; i--)
			{
				tmp += path[i];
				if (path[i] == '.')
					break;
			}
			std::reverse(tmp.begin(), tmp.end());
			return tmp;
		}
		
		int	find_path(std::string path, Config_Server c)
		{
			for (size_t i = 0; i < c.location.size(); i++)
			{
				if (!c.location[i].path.compare(path))
					return i;
			}
			return -1;
		}

		std::string	read_path(std::string path, int code, Location location)
		{
			if (debug)
				std::cout << YELLOW << path << " - " << code << RESET << std::endl;
			std::ifstream	myfile(path);
			std::string		buff;
			std::string		out;
			std::string 	tmp;
			this->res_code = code;

			if (code == 200)
				this->intestation = "HTTP/1.1 200 OK";
			else if (code == 404)
				this->intestation = "HTTP/1.1 404 NOT FOUND";
			else if (code == 400)
				this->intestation = "HTTP/1.1 400 BAD REQUEST";
			else if (code == 401)
				this->intestation = "HTTP/1.1 401 UNAUTORIZED";
			else if (code == 405)
				this->intestation = "HTTP/1.1 405 METHOD NOT ALLOWED";
			else if (code == 301)
			{
				this->intestation = "HTTP/1.1 301 MOVED PERMANENTLY\n";
				this->intestation += "Location: " + location.redirect + "\n";
				this->redirection = true;
				return "";
			}
			
			if (myfile.good())
			{
				//std::cout << BOLDMAGENTA << location.cgi_extension.compare(extension(path)) << RESET << std::endl;
				//std::cout << BOLDMAGENTA  << extension(path) << std::endl  << location.cgi_extension << std::endl << RESET << std::endl;
				if (code == 200 && location.autoindex == true && (tmp = generate_autoindex(path)) != "")
					return (tmp);
				if (location.cgi_path != "" && location.cgi_extension != "" && !location.cgi_extension.compare(extension(path)))
				{
					if(!this->request.content_type.compare(0, 9, "test/file"))
					{
						std::cout << "PARSARE IL BODY E REINSERIRLO NELLA RISPOSTA" << std::endl;
						out = CgiManager::solve_bla_string(this->request.body, this->request, location.cgi_path) + "\n";
					}
					else
						out = CgiManager::solve_all(path, this->request, location.cgi_path, location.cgi_extension);
				}
				else
				{
					while (getline(myfile, buff))
						out += buff + "\n";
				}
				if (location.client_max_body_size != -1 && (size_t)location.client_max_body_size < out.length())
				{
					this->intestation = "HTTP/1.1 413 PAYLOAD TOO LARGE";
					this->oversize = true;
				}
				return out;
			}
			return "";
		}

		bool page_allow(std::vector<std::string> methods, std::string method)
		{
			if (methods.size() == 0)
				return true;
			for (size_t i = 0; i < methods.size(); i++)
			{
				//std::cout << BLUE << (methods[i] == method) << RESET << std::endl;
				if (methods[i] == method)
					return true;
			}
			return false;
		}

		//GENERA IL BODY DI RISPOSTA IN BASE ALLA RICHIESTA EFFETTUATA
		std::string	take_body(Config_Server c, Request r)
		{
			if (!(r.is_valid()) && read_path(c.error_pages.getPath(400), 400, Location()) != "")
				return read_path(c.error_pages.getPath(400), 400, Location());
			else if (!(r.is_valid()))
				return DEFAULT_400;
			//Start cerca l indice della path / nel vector location
			int	start = this->find_path("/", c);
			//Verra utilizzata per creare path complesse piu avanti (è una temp)
			std::string	subpath = "";
			//Ciclo per tutte le location parsate dal config file,
			//alla ricerca della location che corrisponde alla path della richiesta
			for (size_t i = 0; i < c.location.size(); i++)
			{
				//Ciclo all' interno della lista di index parsata dal config file 
				//line condfig ex. index index.html index.php index;
				for (size_t k = 0; k < c.location[i].index.size(); k++)
				{
					//controllo se il path della location ex "/ciccio"è contenuto nel path
					//della richiesta e la size della richesta è uguale alla size della path
					//(ne consegue che path della richiesta e path della location siano uguali)
					//infine controllo se la root (traduzione della path sulla nostra macchina) 
					//della richiesta può essere letta con successo (esiste quindi
					//il file che chiediamo)
					if (debug == true)
						std::cout << "PATH1 : " << c.location[i].root + c.location[i].index[k] << " || " << r.path << " c " << c.location[i].path << " TF: " << (r.path.compare(c.location[i].path)) << std::endl;
					if (!r.path.compare(c.location[i].path)
						&& (read_path(c.location[i].root + c.location[i].index[k], 200, c.location[i]) != "" 
						|| c.location[i].redirect != ""))
					{
						if (debug == true)
							std::cout << "METODO : |" << r.method << "|" << std::endl;
						//IF: se la pagina ci è stata richiesta con un method da noi accettato e il file
						//può essere aperto con successo allora ritorno risposta OK 200 + contenuto file
						//ELSE IF: controllo se esiste una path per l errore 405 forbidden, se esiste 
						//ritorno risposta 405 + contenuto file
						//ELSE: in tutti gli altri casi torno una default path
						if (this->page_allow(c.location[i].method, r.method) && c.location[i].redirect == "")
							return read_path(c.location[i].root + c.location[i].index[k], 200, c.location[i]);
						else if (c.location[i].redirect != "")
							return	read_path("", 301 , c.location[i]);
						else if (read_path(c.error_pages.getPath(405), 405, c.location[i]) != "")
							return	read_path(c.error_pages.getPath(405), 405, c.location[i]);
						else
							return DEFAULT_405;
					}
				}
				//se invece nella path della richiesta è possibile trovare la path della location
				//alla posizione 0, ma non hanno la stessa lunghezza ex. /ciccio /ciccio/pasticcio
				//allora entra nell if
				if (r.path.find(c.location[i].path) == 0)
				{
					//La subpath non farà altro che concatenare la root della path trovata + il resto della path 
					//restante ex. /ciccio/pasticcio -> /www/pasticcio 
					if (debug == true)
						std::cout << "PATH2 : " << r.path.substr(c.location[i].path.length()) << std::endl;
					subpath = c.location[i].root.substr(0, c.location[i].root.length() - 1) + r.path.substr(c.location[i].path.length());
					if (debug == true)
						std::cout << "PATH2 : " << subpath << std::endl;
					//Vedo se effettivamente è possibile trovare il file alla path generata
					if (read_path(subpath, 200, c.location[i]) != "" || c.location[i].redirect != "")
					{
						//IF: se la pagina ci è stata richiesta con un method da noi accettato e il file
						//può essere aperto con successo allora ritorno risposta OK 200 + contenuto file
						//ELSE IF: controllo se esiste una path per l errore 405 forbidden, se esiste 
						//ritorno risposta 405 + contenuto file
						//ELSE: in tutti gli altri casi torno una default path
						if (debug == true)
							std::cout << "METODO : |" << r.method << "|" << std::endl;
						if (this->page_allow(c.location[i].method, r.method) && c.location[i].redirect == "")
							return read_path(subpath, 200, c.location[i]);
						else if (c.location[i].redirect != "")
							return	read_path("", 301 , c.location[i]);
						else if (read_path(c.error_pages.getPath(405), 405, c.location[i]) != "")
							return	read_path(c.error_pages.getPath(405), 405, c.location[i]);
						else
							return DEFAULT_405;
					}
				}
				//std::cout << i << std::endl;
				//std::cout << c.location.size() << std::endl;
			}
			//Nel caso in cui non sia stato possibile trovare una path compatibile con la path richiesta
			//si procederà, in ultima istanza, creando una subpath risultante dalla root della 
			//location "/" + la path della richiesta ex. root = /mimmo path = /ciccio/pasticcio
			//risultante = /mimmo/ciccio/pasticcio
			subpath =c.location[start].root + r.path;
			if (debug == true)
				std::cout << "PATH3 : " << subpath << std::endl;
			//se il file esiste alla subpath generata entra nell if
			if (read_path(subpath, 200, c.location[start]) != "" || c.location[start].redirect != "")
			{
				//IF: se la pagina ci è stata richiesta con un method da noi accettato e il file
				//può essere aperto con successo allora ritorno risposta OK 200 + contenuto file
				//ELSE IF: controllo se esiste una path per l errore 405 forbidden, se esiste 
				//ritorno risposta 405 + contenuto file
				//ELSE: in tutti gli altri casi torno una default path
				if (this->page_allow(c.location[start].method, r.method) && c.location[start].redirect == "")
					return read_path(subpath, 200, c.location[start]);
				else if (c.location[start].redirect != "")
							return	read_path("", 301 , c.location[start]);
				else if (read_path(c.error_pages.getPath(405), 405, c.location[start]) != "")
					return	read_path(c.error_pages.getPath(405), 405, c.location[start]);
				else
					return DEFAULT_405;
			}
			//IF: controllo se esiste una path per l errore 405 forbidden, se esiste 
			//ritorno risposta 404 + contenuto file
			//ELSE: ritorno una pagina di errore 404 di default
			if (read_path(c.error_pages.getPath(404), 404, Location()) != "")
				return	read_path(c.error_pages.getPath(404), 404, Location());
			return DEFAULT_404;
		}

		std::string generate_autoindex(std::string directory)
		{
			struct dirent *entry;
			std::string tmp;
			std::string abody;
			std::string link;

			abody = "<!DOCTYPE html><body><h1>Index of " + request.path + "</h1>";
			std::cout << RED << request.host << RESET << std::endl;
			DIR *dir = opendir(directory.c_str());
			// se non e' una directory esce
			if (dir == NULL)
				return ("");
			while ((entry = readdir(dir)) != NULL) 
			{
				// nasconde file
				if(entry->d_name[0] == '.')
					continue ;
				tmp = entry->d_name;
				abody += "<li><a href=\"" + request.method_path;
				if (abody[abody.length() - 1] != '/' )
					abody += "/";
				abody += tmp + "\">" + tmp;
				// mette "/" dopo cartelle
				if (entry->d_type == DT_DIR)
					abody += "/";
				abody += "</a></li>";
				//std::cout << RED << tmp << " : " << (entry->d_type == DT_DIR) << std::endl;
			}
			abody += "</body></html>";
    		closedir(dir);
			return (abody);
		}

		std::string generate_cookie()
		{
			std::string ret;
			const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

			for (int i = 0; i < 50; ++i) 
				ret.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
			return ret;
		}

	public:
		int 				res_code;
		std::string	out;
		Response(){}
		Response(Config_Server c, Request r) : conf(c)
		{
			this->redirection = false;
			this->oversize = false;
			this->request = r;
			this->body = take_body(c, r);
			if (!this->redirection)
			{
				this->connection =  "Connection: " + r.connection;
				this->content_len = "Content-Length: " + std::to_string(this->body.length());
				this->content_type = "Content-Type: text/html";
				this->out = "";
				if (this->oversize || r.method == "HEAD")
					this->out = this->intestation + "\n" +this->connection + "\n\n";
				else
				{
					this->out = this->intestation + "\n" + this->content_type + "\n" + this->content_len + "\n" +this->connection;
					// generates cookie if needed
					if (r.cookie.find("_id=") == std::string::npos)
						this->out += "\nSet-Cookie: _id=" + generate_cookie(); //+ "\n";
					this->out += "\n\n" + this->body;
				}
			}
			else
			{
				this->out = this->intestation + "\n";
			}
		}
		//~Response();
};
