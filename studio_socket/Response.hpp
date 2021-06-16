#pragma once

#include "Config.hpp"
#include "Request.hpp"
#include <dirent.h>
#include <sstream>

#define DEFAULT_404 "<html><h1>ERROR 404 NOT FOUND</h1></html>"
#define DEFAULT_401 "<html><h1>ERROR 401 UNAUTORIZED</h1></html>"
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
			//std::cout << YELLOW << path << RESET << std::endl;
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

		bool page_allow(std::vector<std::string> methods, std::string method)
		{
			if (methods.size() == 0)
				return true;
			//std::cout << BLUE << methods.size() << RESET << std::endl;
			for (int i = 0; i < methods.size(); i++)
				if (methods[i] == method)
					return true;
			return false;
		}

		//GENERA IL BODY DI RISPOSTA IN BASE ALLA RICHIESTA EFFETTUATA
		std::string	take_body(Config_Server c, Request r)
		{
			if (!(r.is_valid()))
				return read_path(c.error_pages.getPath(404), 400);
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
					if (r.path.find(c.location[i].path) == 0 && r.path.size() == c.location[i].path.size()
						&& read_path(c.location[i].root + c.location[i].index[k], 200) != "")
					{
						//IF: se la pagina ci è stata richiesta con un method da noi accettato e il file
						//può essere aperto con successo allora ritorno risposta OK 200 + contenuto file
						//ELSE IF: controllo se esiste una path per l errore 401 forbidden, se esiste 
						//ritorno risposta 401 + contenuto file
						//ELSE: in tutti gli altri casi torno una default path
						if (this->page_allow(c.location[i].method, r.method)
							&& read_path(c.location[i].root + c.location[i].index[k], 200) != "")
							return read_path(c.location[i].root + c.location[i].index[k], 200);
						else if (read_path(c.error_pages.getPath(401), 401) != "")
							return	read_path(c.error_pages.getPath(401), 401);
						else
							return DEFAULT_401;
					}
				}
				//se invece nella path della richiesta è possibile trovare la path della location
				//alla posizione 0, ma non hanno la stessa lunghezza ex. /ciccio /ciccio/pasticcio
				//allora entra nell if
				if (r.path.find(c.location[i].path) == 0)
				{
					//La subpath non farà altro che concatenare la root della path trovata + il resto della path 
					//restante ex. /ciccio/pasticcio -> /www/pasticcio 
					subpath = r.path.substr(c.location[i].path.length(), r.path.size() - c.location[i].path.length());
					//Vedo se effettivamente è possibile trovare il file alla path generata
					if (read_path(subpath, 200) != "")
					{
						//IF: se la pagina ci è stata richiesta con un method da noi accettato e il file
						//può essere aperto con successo allora ritorno risposta OK 200 + contenuto file
						//ELSE IF: controllo se esiste una path per l errore 401 forbidden, se esiste 
						//ritorno risposta 401 + contenuto file
						//ELSE: in tutti gli altri casi torno una default path
						if (this->page_allow(c.location[i].method, r.method))
							return read_path(subpath, 200);
						else if (read_path(c.error_pages.getPath(401), 401) != "")
							return	read_path(c.error_pages.getPath(401), 401);
						else
							return DEFAULT_401;
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
			//se il file esiste alla subpath generata entra nell if
			if (read_path(subpath, 200) != "")
			{
				//IF: se la pagina ci è stata richiesta con un method da noi accettato e il file
				//può essere aperto con successo allora ritorno risposta OK 200 + contenuto file
				//ELSE IF: controllo se esiste una path per l errore 401 forbidden, se esiste 
				//ritorno risposta 401 + contenuto file
				//ELSE: in tutti gli altri casi torno una default path
				if (this->page_allow(c.location[start].method, r.method))
					return read_path(subpath, 200);
				else if (read_path(c.error_pages.getPath(401), 401) != "")
					return	read_path(c.error_pages.getPath(401), 401);
				else
					return DEFAULT_401;
			}
			//IF: controllo se esiste una path per l errore 401 forbidden, se esiste 
			//ritorno risposta 404 + contenuto file
			//ELSE: ritorno una pagina di errore 404 di default
			if (read_path(c.error_pages.getPath(404), 404) != "")
				return	read_path(c.error_pages.getPath(404), 404);
			return DEFAULT_404;
		}

		void generate_autoindex(Config_Server c, Request r)
		{
			struct dirent *entry;
			std::string tmp;
			std::string directory;
			// posso passarmi direttamente start da take_body?3
			//NO NON È QUELLO CHE PENSI
			int	start;
			//std::cout << "PPP  <" << r.path << ">" << std::endl;
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
				//std::cout << RED << tmp << " : " << (entry->d_type == DT_DIR) << std::endl;
			}
			this->body += "</body></html>";
    		closedir(dir);
		}

	public:
		std::string	out;

		Response(Config_Server c, Request r) : conf(c)
		{
			this->request = r;
			//generate_autoindex(c, r);
			this->body = take_body(c, r);
			this->connection =  "Connection: " + r.connection;
			this->content_len = "Content-Length: " + std::to_string(this->body.length());
			this->content_type = "Content-Type: text/html";

			this->out = this->intestation + "\n" + this->content_type + "\n" + this->content_len + "\n" +this->connection + "\n\n" + this->body + "\n";
		}
		//~Response();
};
