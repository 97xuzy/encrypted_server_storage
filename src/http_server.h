#include "network_server.h"
#include "http_parser_wrapper.h"
#include "epoll_wrapper.h"
#include <string>
#include <vector>
#include <map>
#include <functional>

class HTTPHandler;

class HTTPServer
{
protected:
    ServerSocket serv_sock;
    HTTP_Parser parser;
    EventPoll epoll;
    std::vector<HTTPHandler> handlers;
    std::vector<int> fd_with_events;

    void handle_incoming_conn();
    void handle_incoming_request(const ServerConnection &conn);
    bool new_events();
public:
    HTTPServer(int port);
    void run();
};


/*
class HTTPHandler_generic
{
public:
    virtual std::function<std::string(const std::map<std::string, std::string>&, const std::string&)> on_request;
    virtual bool match(const std::map<std::string, std::string>& header_fields);
};
*/
class HTTPHandler// : public HTTPHandler_generic
{
private:

    static constexpr char http_header[] = "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Transfer-Encoding: chunked\r\n\r\n";
    static constexpr char file_root_path[] ="/var/www/encrypted_server_storage";
    std::vector<std::string> host;
    std::string on_request_func(const std::map<std::string, std::string> &header_field, const std::string& request_body);
    std::string read_file(const std::string &filename);
protected:
    static std::string chunked_encoding(const std::string &);
    static std::string error_404_response();
public:
    std::function<std::string(const std::map<std::string, std::string>&, const std::string&)> on_request;
    HTTPHandler();
    ~HTTPHandler();
    bool match(const std::map<std::string, std::string>& header_fields);
};
