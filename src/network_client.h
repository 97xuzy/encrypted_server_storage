#pragma once

#include <string>

class ClientSocket
{
public:
    std::string ip_addr;
    int port;
    virtual void connect() = 0;
    virtual void send(const std::string &data) = 0;
    virtual std::string receive_all() = 0;
    virtual std::string receive() = 0;
    virtual void disconnect() = 0;
};
