#ifdef __unix__
#include "network_server.h"

#include <string>
using std::string;
#include <stdexcept>
using std::runtime_error;
#include <vector>
using std::vector;

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

ServerSocket::ServerSocket(const string &ip_addr, int port)
{
    this->ip_addr = ip_addr;
    this->port = port;

    // Create Socket
    this->server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(this->server_sock == -1)
    {
        throw new runtime_error("fail to create socket");
    }

    // Bind Socket with IP addr and Port
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    struct in_addr addr;
    if(inet_aton(ip_addr.c_str(), &addr) == 0)
    {
        throw new runtime_error("invalid ip address");
    }
    serv_addr.sin_family = AF_INET;  // IPv4 Addr
    serv_addr.sin_addr.s_addr = addr.s_addr;
    serv_addr.sin_port = htons(port);
    if(bind(this->server_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        throw new runtime_error("fail to bind");
    }

    //this->clnt_sock_list = new vector<int>();
}

ServerSocket::~ServerSocket()
{
    close(this->server_sock);
}

void ServerSocket::listen()
{
    if(::listen(this->server_sock, 20) == -1)
    {
        throw new runtime_error("fail to listen");
    }
}

ServerConnection ServerSocket::accept()
{
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = ::accept(this->server_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if(clnt_sock == -1)
    {
        throw new runtime_error("fail to accept new connection");
    }
    return ServerConnection(clnt_sock);
}

ServerConnection::ServerConnection(int clnt_sock)
{
    this->clnt_sock = clnt_sock;
}

void ServerConnection::send(const string &data) const
{
    // Send Response
    ::write(this->clnt_sock, data.c_str(), data.length());
}

std::string ServerConnection::recv() const
{
    string data;
    // Read Request
    char buffer[1024];
    int byte_read = 0;
    do
    {
        byte_read = ::read(this->clnt_sock, buffer, sizeof(buffer));
        data.append(buffer, byte_read);

        if(byte_read < 0)
        {
            throw new runtime_error("fail to read from socket");
        }

    }
    while(byte_read == 1024);
    return data;
}

void ServerConnection::close()
{
    ::close(this->clnt_sock);
}




#endif
