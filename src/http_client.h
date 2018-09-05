#pragma once

#if defined(__unix__)
#include "network_client_posix.h"
#elif defined(_WIN32)
#include "network_client_win.h"
#endif

#include "../http-parser/http_parser.h"
#include <string>
#include <utility>
#include <optional>

class HTTPClientAuthentication
{
public:
    std::optional<std::string> need_auth(const std::string &response);
    std::string authenticate(const std::string &uri, const std::string &credential);
};

class HTTPClient
{
private:
    http_parser parser;
    http_parser_settings parser_settings;
    HTTPClientAuthentication auth;
#if defined(__unix__)
    ClientSocketPOSIX socket;
#elif defined(_WIN32)
    ClientSocketWin socket;
#else
#error "undefined platform"
#endif

    static std::string response_body;
    static int on_status(http_parser *p, const char *at, size_t length);
    static int on_header_field(http_parser *p, const char *at, size_t length);
    static int on_body(http_parser *p, const char *at, size_t length);
    static int on_complete(http_parser *p);

public:
    enum HTTP_METHOD {GET, POST};
    HTTPClient(const std::string &ip_addr);
    void send_request(const std::string &uri, const std::string &host);
    std::pair<int, std::string> receive_response_body();
    std::string receive_response_str();
};



