#pragma once

#include "../http-parser/http_parser.h"
#include <string>
#include <map>
#include <utility>

class HTTP_Parser
{
public:
    http_parser parser;
    http_parser_settings parser_settings;

    static std::map<std::string, std::string> header_fields;
    static std::string last_header_field;
    static std::string request_body;
    static int on_url(http_parser *p, const char *at, size_t length);
    static int on_header_field(http_parser *p, const char *at, size_t length);
    static int on_header_value(http_parser *p, const char *at, size_t length);
    static int on_body(http_parser *p, const char *at, size_t length);
public:
    HTTP_Parser();
    ~HTTP_Parser();
    std::pair<std::map<std::string, std::string>, std::string> parse(const std::string &request_str);
};
