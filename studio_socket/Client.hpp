#pragma once

#include <string>
#include <arpa/inet.h>

class Client
{
    public:
    int fd_server;
    int index_server;
    int fd_client;
    socklen_t addrlen;
	struct sockaddr_in clientAddr;
    std::string buffer;

    Client(){}
    Client(int _fd_server, int _index_server, int _fd_client, socklen_t _addrlen, struct sockaddr_in _clientAddr) : fd_server(_fd_server), index_server(_index_server),fd_client(_fd_client), addrlen(_addrlen), clientAddr(_clientAddr), buffer("") {}

    void appendBuffer(const char *new_buffer, int len)
    {
        buffer.append(new_buffer, 0, len);
    }

    bool header_ok(void)
    {
        if (buffer.find("\r\n\r\n") != std::string::npos)
            return true;
        return false;
    }

    std::string getIp(void)
    {
        return inet_ntoa(clientAddr.sin_addr);
    }

    int header_len()
    {
        return 0;
    }


};
