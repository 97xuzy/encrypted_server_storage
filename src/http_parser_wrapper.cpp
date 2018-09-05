#include "http_parser_wrapper.h"
#include "../http-parser/http_parser.h"
#include <string>
using std::string;
#include <map>
using std::map;
#include <utility>
using std::pair;


std::map<std::string, std::string> HTTP_Parser::header_fields;
std::string HTTP_Parser::last_header_field;
std::string HTTP_Parser::request_body;

HTTP_Parser::HTTP_Parser()
{
    this->parser_settings = {
        nullptr, // on_message_begin;
        HTTP_Parser::on_url, // on_url;
        nullptr, // on_status;
        HTTP_Parser::on_header_field, // on_header_field;
        HTTP_Parser::on_header_value, // on_header_value;
        nullptr, // on_headers_complete;
        HTTP_Parser::on_body, // on_body;
        nullptr, // on_message_complete;
        nullptr, // on_chunk_header;
        nullptr  // on_chunk_complete;
    };

    http_parser_init(&this->parser, HTTP_REQUEST);
}

HTTP_Parser::~HTTP_Parser()
{
}

int HTTP_Parser::on_url(http_parser *p, const char *at, size_t length)
{
    HTTP_Parser::header_fields.emplace(std::make_pair(string("request_uri"), std::string(at, length)));
    return 0;
}

int HTTP_Parser::on_header_field(http_parser *p, const char *at, size_t length)
{
    HTTP_Parser::last_header_field = string(at, length);
    return 0;
}

int HTTP_Parser::on_header_value(http_parser *p, const char *at, size_t length)
{
    HTTP_Parser::header_fields.emplace(std::make_pair(HTTP_Parser::last_header_field, std::string(at, length)));
    HTTP_Parser::last_header_field.clear();
    return 0;
}

int HTTP_Parser::on_body(http_parser *p, const char *at, size_t length)
{
    HTTP_Parser::request_body = string(at, length);
    return 0;
}

pair<map<string, string>, string> HTTP_Parser::parse(const std::string &request_str)
{
    http_parser_init(&this->parser, HTTP_REQUEST);

    HTTP_Parser::header_fields.clear();
    HTTP_Parser::request_body.clear();
    http_parser_init(&this->parser, HTTP_REQUEST);
    size_t parsed = http_parser_execute(&this->parser, &this->parser_settings,
            request_str.c_str(), request_str.length());
    if(request_str.length() != parsed)
    {
        HTTP_Parser::header_fields.clear();
        HTTP_Parser::request_body.clear();
    }
    return std::make_pair(HTTP_Parser::header_fields, HTTP_Parser::request_body);
}