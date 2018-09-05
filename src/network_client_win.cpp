#ifdef _WIN32

#include "network_client_win.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
using std::runtime_error;
#include <string>
using std::string;

#pragma comment(lib, "ws2_32.lib")

ClientSocketWin::ClientSocketWin(const string &ip_addr, int port)
{
    this->ip_addr = ip_addr;
    this->port = port;
    if((this->ip = inet_addr(ip_addr.c_str()) == INADDR_NONE)
    {
        throw new runtime_error("illegal ip address");
    }
}

void ClientSocketWin::connect()
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        throw new runtime_error("fail to init winsock dll");
    }
    if ((this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        throw new runtime_error("fail to create socket");
    }
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.S_un.S_addr = this->ip;
    serverAddress.sin_port = htons(this->port);
    if (connect(this->sock, (sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        throw new runtime_error("fail to connect");
    }
}

void ClientSocketWin::send(const string &data)
{
    const char* c_str = data.c_str();
    if (send(sock, c_str, strlen(c_str), 0) == SOCKET_ERROR)
    {
        throw new runtime_error("fail to send data");
    }
}

string ClientSocketWin::receive_all()
{
    string data;
    char buffer[1024];
    int bytes = sizeof(buffer);
    while(bytes == sizeof(buffer)) {
        if ((bytes = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)
        {
            throw new runtime_error("fail to receive data/n");
        }
        data.append(buffer, bytes);
    }
    return data;
}

string ClientSocketWin::receive()
{
    string data;
    char buffer[1024];
    int bytes = sizeof(buffer);
    while(bytes == sizeof(buffer)) {
        if ((bytes = recv(sock, buffer, sizeof(buffer), 0)) == SOCKET_ERROR)
        {
            throw new runtime_error("fail to receive data/n");
        }
        data.append(buffer, bytes);
    }
    return data;
}

void ClientSocketWin::disconnect()
{
    closesocket(this->sock);
    WSACleanup();
}

ClientSocketWin::~ClientSocketWin()
{
    WSACleanup();
}

int main1(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s IPAddress PortNumber/n", argv[0]);
        exit(-1);
    }
    unsigned long ip;
    if ((ip = inet_addr(argv[1])) == INADDR_NONE)
    {
        printf("illegal ip address: %s", argv[1]);
        exit(-1);
    }
    short port;
    if ((port = atoi(argv[2])) == 0)
    {
        printf("incorrect port number");
        exit(-1);
    }
    printf("Connecting to %s:%d....../n", inet_ntoa(*(in_addr *)&ip), port);
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("fail to init winsock dll");
        exit(-1);
    }
    SOCKET sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        printf("fail to create socket");
        exit(-1);
    }
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.S_un.S_addr = ip;
    serverAddress.sin_port = htons(port);
    if (connect(sock, (sockaddr *)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        printf("fail to connect");
        exit(-1);
    }
    char buf[4096];
    while (1)
    {
        printf(">");
        scanf("%4096s", buf);
        if (send(sock, buf, strlen(buf), 0) == SOCKET_ERROR)
        {
            printf("fail to send data");
            exit(-1);
        }
        int bytes;
        if ((bytes = recv(sock, buf, sizeof(buf), 0)) == SOCKET_ERROR)
        {
            printf("fail to receive data/n");
            exit(-1);
        }
        buf[bytes] = '/0';
        printf("Message from %s: %s/n", inet_ntoa(serverAddress.sin_addr), buf);
    }
    WSACleanup();
    return 0;
}

#endif
