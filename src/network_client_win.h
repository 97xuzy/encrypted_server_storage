#ifdef _WIN32

#pragma once
#include "network_client.h"
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdexcept>
#include <string>

class ClientSocketWin : public ClientSocket
{
private:
    unsigned long ip;
    SOCKET sock;
public:
    std::string ip_addr;
    int port;
    ClientSocketWin(const std::string &ip_addr, int port);
    ~ClientSocketWin();
    void connect();
    void send(const std::string &data);
    std::string receive_all();
    std::string receive();
    void disconnect();
};

#endif
