#ifdef __unix__

#include "network_client_posix.h"
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <stdexcept>
using std::runtime_error;
#include <string>
using std::string;

ClientSocketPOSIX::ClientSocketPOSIX(const string &ip_addr, int port) : ip_addr(ip_addr)
{
    this->port = port;
    if((this->ip = inet_addr(ip_addr.c_str())) == INADDR_NONE)
    {
        throw new runtime_error("illegal ip address");
    }
}

void ClientSocketPOSIX::connect()
{
    this->sock = 0;
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        throw new runtime_error("Socket creation error");
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, ip_addr.c_str(), &serv_addr.sin_addr) <= 0) 
    {
        throw new runtime_error("\nInvalid address/ Address not supported \n");
    }
  
    if (::connect(this->sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        throw new runtime_error("\nConnection Failed \n");
    }

}

void ClientSocketPOSIX::send(const string &data)
{
    if(::send(this->sock , data.c_str(), data.length(), 0 ) == -1)
    {
        throw new runtime_error("fail to send data");
    }
}

string ClientSocketPOSIX::receive_all()
{
    string data;
    char buffer[1024];

    ssize_t byte_read = 0;
    do
    {
        byte_read = ::read(sock, buffer, sizeof(buffer));

        if(byte_read < 0) return data;

        data.append(buffer, byte_read * sizeof(buffer));
    }
    while(byte_read == sizeof(buffer));
    return data;
}

string ClientSocketPOSIX::receive()
{
    string data;
    char buffer[1024];

    ssize_t byte_read = 0;
    do
    {
        byte_read = ::read(sock, buffer, sizeof(buffer));

        if(byte_read < 0) return data;

        data.append(buffer, byte_read * sizeof(buffer));
    }
    while(byte_read == sizeof(buffer));
    return data;
}

void ClientSocketPOSIX::disconnect()
{
    ::close(this->sock);
}

ClientSocketPOSIX::~ClientSocketPOSIX()
{
}

#endif
