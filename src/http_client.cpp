#include "http_client.h"

#if defined(__unix__)
#include "network_client_posix.h"
#elif defined(_WIN32)
#include "network_client_win.h"
#endif

#include "../http-parser/http_parser.h"
#include <string>
using std::string;
#include <utility>

#include <iostream>

string HTTPClient::response_body;

#if defined(__unix__)
HTTPClient::HTTPClient(const std::string &ip_addr) : socket(ip_addr, 8183)
#elif defined(_WIN32)
HTTPClient::HTTPClient(const std::string &ip_addr) : socket(ip_addr, 8183)
#else
#error "undefined platform"
#endif
{
    this->parser_settings = {
        nullptr,     // on_message_begin;
        nullptr,     // on_url;
        HTTPClient::on_status,     // on_status;
        HTTPClient::on_header_field,    // on_header_field;
        nullptr,     // on_header_value;
        nullptr,     // on_headers_complete;
        HTTPClient::on_body,     // on_body;
        HTTPClient::on_complete, // on_message_complete;
        nullptr,     // on_chunk_header;
        nullptr      // on_chunk_complete;
    };

    ::http_parser_init(&this->parser, HTTP_RESPONSE);
}

int HTTPClient::on_status(http_parser *p, const char *at, size_t length)
{
    if(p->status_code != 200)
    {
        http_parser_pause(p, 1);
    }
    return 0;
}

int HTTPClient::on_header_field(http_parser *p, const char *at, size_t length)
{
    //std::cout << string(at, length) << '\n';
    return 0;
}

int HTTPClient::on_body(http_parser *p, const char *at, size_t length)
{
    //std::cout << string(at, length);
    HTTPClient::response_body.append(string(at, length));
    return 0;
}

int HTTPClient::on_complete(http_parser *p)
{
    std::cout << "CComplete\n";
    return 0;
}

void HTTPClient::send_request(const string &uri, const string &host)
{
    string request("GET ");
    request.append(uri);
    request.append(" HTTP/1.1\r\n");

    request.append("Host: ");
    request.append(host);
    request.append("\r\n\r\n");

    this->socket.send(request);
}

std::pair<int, string> HTTPClient::receive_response_body()
{
    string response = this->socket.receive_all();
    /*
    string response("HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: text/plain\r\n\r\n"
                    "4\r\nWiki\r\n5\r\npedia\r\nE\r\n in\r\n\r\nchunks.\r\n0\r\n\r\n");
                    */
    size_t parsed = ::http_parser_execute(&this->parser, &this->parser_settings,
            response.c_str(), response.length());

    if(parsed == response.length())
    {
        return std::make_pair<int, string>((int)parser.status_code, string());
    }

    return std::make_pair<int, string>((int)parser.status_code, string(HTTPClient::response_body));
}

string HTTPClient::receive_response_str()
{
    return this->socket.receive_all();
}

std::optional<std::string> HTTPClientAuthentication::need_auth(const std::string &response)
{
    // "WWW-Authenticate"
    //return {};
    return response;
}

std::string HTTPClientAuthentication::authenticate(const std::string &uri, const std::string &credential)
{
    string request("GET ");
    request.append(uri);
    request.append(" HTTP/1.1\r\n");

    request.append("Authorization: Basic ");
    request.append(credential);
    request.append("\r\n\r\n");

    return request;
}

/*
string data = string(
    "HTTP/1.1 200 OK\r\n"
    "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n"
    "Server: Apache/2.2.14 (Win32)\r\n"
    "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\r\n"
    "Content-Length: 88\r\n"
    "Content-Type: text/html\r\n"
    "Connection: Closed\r\n\r\nHello World\r\n");
*/
/*
string data = string(
    "POST /joyent/http-parser HTTP/1.1\r\n"
    "Host: github.com\r\n"
    "DNT: 1\r\n"
    "Accept-Encoding: gzip, deflate, sdch\r\n"
    "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
    "AppleWebKit/537.36 (KHTML, like Gecko) "
    "Chrome/39.0.2171.65 Safari/537.36\r\n"
    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
    "Connection: keep-alive\r\n"
    "Transfer-Encoding: chunked\r\n"
    "Cache-Control: max-age=0\r\n\r\nb\r\nhello world\r\n0\r\n");
*/

