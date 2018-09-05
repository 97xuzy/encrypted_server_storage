#include "http_server.h"

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <map>
using std::map;
#include <iostream>
#include <fstream>
using std::ifstream;
#include <sstream>
#include <utility>
using std::pair;
#include <functional>
using std::function;

HTTPServer::HTTPServer(int port) : serv_sock(string("127.0.0.1"), port)
{
    this->epoll.add_fd(this->serv_sock.fd());
    this->handlers.push_back(HTTPHandler());
}

bool HTTPServer::new_events()
{
    this->fd_with_events.clear();
    this->fd_with_events = this->epoll.wait_events();
    return this->fd_with_events.size() > 0;
}

void HTTPServer::handle_incoming_conn()
{
    ServerConnection conn = this->serv_sock.accept();
    this->epoll.add_fd(conn.fd());
}

void HTTPServer::handle_incoming_request(const ServerConnection &conn)
{
    string request_str = conn.recv();

    auto p = this->parser.parse(request_str);
    map<string, string> header_fields = std::get<0>(p);
    string body = std::get<1>(p);

    // Handle new request
    for (auto handler : this->handlers)
    {
        if (handler.match(header_fields))
        {
            conn.send(handler.on_request(header_fields, body));
            break;
        }
    }
}

void HTTPServer::run()
{
    this->serv_sock.listen();

    while(true)
    {
        while(! this->new_events());

        vector<int> fd_with_events = this->epoll.wait_events();
        for(int fd : fd_with_events)
        {
            if(fd == this->serv_sock.fd())
            {
                this->handle_incoming_conn();
            }
            else
            {
                this->handle_incoming_request(ServerConnection(fd));
            }
        }


    }
}

HTTPHandler::HTTPHandler()
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    this->on_request = std::bind(&HTTPHandler::on_request_func, this, _1, _2);
}

HTTPHandler::~HTTPHandler()
{
}

string HTTPHandler::read_file(const string &filename)
{

    string full_path(file_root_path);
    full_path.append(filename);

    // open file for reading
    ifstream stream(full_path, std::ios::binary);
    if (!stream.is_open())
    {
        std::cout << "failed to open " << filename << '\n';
        return std::string();
    }
    else
    {
        /*
        std::ifstream t("file.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();
        */
        stream.seekg(0, std::ios::end);
        size_t size = stream.tellg();
        string content(size, ' ');
        stream.seekg(0);
        stream.read(&content[0], size);
        return content;
    }
}

string HTTPHandler::chunked_encoding(const std::string &raw_input)
{
    string encoded_data;
    const size_t chunk_size = 20;

    std::stringstream ss;

    if (raw_input.length() < chunk_size)
    {
        ss << std::hex << raw_input.length();
        std::string chunk_len_hex(ss.str());

        encoded_data.append(chunk_len_hex);
        encoded_data.append("\r\n");
        encoded_data.append(raw_input);
        encoded_data.append("\r\n0\r\n\r\n");
        return encoded_data;
    }

    size_t encoded_len = 0;
    ss << std::hex << chunk_size;
    std::string chunk_len_hex(ss.str());
    for (size_t i = 0; i < raw_input.length() / chunk_size; i++)
    {
        encoded_data.append(chunk_len_hex);
        encoded_data.append("\r\n");
        encoded_data.append(raw_input.substr(encoded_len, chunk_size));
        encoded_data.append("\r\n");
        encoded_len += chunk_size;
    }
    if(raw_input.length() % chunk_size != 0)
    {
        ss.str("");
        ss.clear();
        ss << std::hex << raw_input.length() % chunk_size;
        std::string chunk_len_hex(ss.str());

        encoded_data.append(chunk_len_hex);
        encoded_data.append("\r\n");
        encoded_data.append(raw_input.substr(encoded_len, raw_input.length() % chunk_size));
        encoded_data.append("\r\n");
    }
    encoded_data.append("0\r\n\r\n");

    return encoded_data;
}

string HTTPHandler::error_404_response()
{
    return string("HTTP/1.1 404 Not found\r\n\r\n");
}

string HTTPHandler::on_request_func(const map<string, string> &header_fields, const std::string& request_body)
{
    string request_uri;
    try
    {
        request_uri = header_fields.at(std::string("request_uri"));
    }
    catch(std::out_of_range &e)
    {
        return error_404_response();
    }
    string response(http_header);
    string file_content = read_file(request_uri);
    response.append(chunked_encoding(file_content));

    return response;
}

bool HTTPHandler::match(const map<string, string>& header_fields)
{
    return true;
}


