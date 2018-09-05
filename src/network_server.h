#ifdef __unix__

#include <string>
#include <vector>
#include <memory>
using std::unique_ptr;

class ServerConnection;

class ServerSocket
{
private:
    std::string ip_addr;
    int port;
    int server_sock;
    //std::vector<ServerConnection> clnt_sock_list;
public:
    ServerSocket(const std::string &ip_addr, int port);
    ~ServerSocket();
    void listen();
    ServerConnection accept();
    int fd() { return this->server_sock; }
};

class ServerConnection
{
private:
    int clnt_sock;
public:
    ServerConnection(int clnt_sock);
    void send(const std::string &data) const;
    std::string recv() const;
    void close();
    int fd() { return this->clnt_sock; }
};


#endif