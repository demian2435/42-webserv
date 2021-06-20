#pragma once

#include <string>
#include <sstream>
#include <list>
#include <arpa/inet.h>
#include <chrono>
#include "Request.hpp"
#include "Response.hpp"
#include "Config.hpp"

class Client
{
	public:
	int						fd_server;
	int						index_server;
	int						fd_client;
	socklen_t				addrlen;
	struct sockaddr_in		clientAddr;
	std::string				buffer;
	bool					header;
	Request					req;
	Response				res;
	int						nbytes;
	std::list<std::string>	buffer_list;

	Client(){}
	Client(int _fd_server, int _index_server, int _fd_client, socklen_t _addrlen, struct sockaddr_in _clientAddr) : fd_server(_fd_server), index_server(_index_server),fd_client(_fd_client), addrlen(_addrlen), clientAddr(_clientAddr), buffer(""), header(false) {}

	void appendBuffer(const char *new_buffer)
	{
		std::string temp = new_buffer;
		//buffer.append(new_buffer, 0, nbytes);
		buffer_list.push_back(temp.substr(0, nbytes));
	}

	void mount_buffer(void)
	{
		buffer = "";
		for (std::list<std::string>::iterator i = buffer_list.begin(); i != buffer_list.end(); i++)
			buffer.append(*i);
	}

	bool header_ok(void)
	{
		if (header == false)
		{
			mount_buffer();
			if (buffer.find("\r\n\r\n") != std::string::npos)
				header = true;
		}
		return header;
	}

	bool body_ok(void)
	{
		if (!(req.transfer_encoding.compare(0,7, "chunked")))
			return false;

		int len = 0;
		for (std::list<std::string>::iterator i = buffer_list.begin(); i != buffer_list.end(); i++)
			len += (*i).length();
		len -= getHeader().length();

		if (len < req.content_length)
			return false;
		return true;
	}

	bool isReady(void)
	{
		std::list<std::string>::iterator eee =  buffer_list.end();
		--eee;
		if ((*eee).length() == 1 || (*eee).find("0\r\n\r\n") != std::string::npos || body_ok())
		{
			getRequest();
			return req.is_ready();
		}
		return false;
	}

	std::string getHeader(void)
	{
		mount_buffer();
		return buffer.substr(0, buffer.find("\r\n\r\n")) + "\n";
	}

	void getRequest(void)
	{
		mount_buffer();
		req = Request(buffer);
	}

	void getResponse(Config conf)
	{
		res = Response(conf.server[index_server], req);
	}

	std::string getIp(void)
	{
		return inet_ntoa(clientAddr.sin_addr);
	}

};
