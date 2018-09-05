#ifdef __unix__

#pragma once
#include "network_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <string>

class ClientSocketPOSIX : public ClientSocket
{
private:
    unsigned long ip;
    int sock;
public:
    std::string ip_addr;
    int port;
    ClientSocketPOSIX(const std::string &ip_addr, int port);
    ~ClientSocketPOSIX();
    void connect();
    void send(const std::string &data);
    std::string receive_all();
    std::string receive();
    void disconnect();
};

#endif
