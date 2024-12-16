#include <iostream>

#include "http/request.h"
#include "tcp/server.h"

static void field_parser(http::request::header::field field, std::string_view value, void* p_context);

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    std::cout << "starting server on port 127.0.0.1:80" << std::endl;
    tcp::server my_server("80");

    std::array fields_to_parse{
        http::request::header::field::host,
        http::request::header::field::user_agent,
        http::request::header::field::accept,
        http::request::header::field::connection,
        http::request::header::field::content_type,
        http::request::header::field::content_length
    };

    my_server.start(fields_to_parse, field_parser, nullptr);

    return 0;
}

static void field_parser(http::request::header::field field, std::string_view value, void* p_context)
{
    (void)field;
    (void)value;
    (void)p_context;
    std::cout << "[" << http::request::header::to_string(field) << "] = " << value << std::endl;

    return;
}
