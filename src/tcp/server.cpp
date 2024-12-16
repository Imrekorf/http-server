#include <iostream>
#include <string_view>
#include <winerror.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>

#include "tcp/server.h"
#include "http/response.h"
#include "tcp/detail.h"
#include "http/request.h"

const char html[] = "<html>\r\n"
                    "<head>\r\n"
                    "<title>Hello, world!</title>\r\n"
                    "</head>\r\n"
                    "<body>\r\n"
                    "<h1>Hello, world!</h1>\r\n"
                    "</body>\r\n"
                    "</html>\r\n\r\n";

using std::string_view_literals::operator""sv;

namespace tcp {
    server::server(std::string_view port)
    {
        WSADATA wsa;

        assert(WSAStartup(MAKEWORD(2, 2), &wsa) == 0);

        addrinfo* res = NULL;
        addrinfo hints;

        ZeroMemory(&hints, sizeof(hints) );

        hints.ai_family   = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags    = AI_PASSIVE;

        assert(getaddrinfo(NULL, port.data(), &hints, &res) == 0);

        m_socket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        assert(m_socket != INVALID_SOCKET);
        assert(bind(m_socket, res->ai_addr, (int)res->ai_addrlen) != SOCKET_ERROR);
    }

    void server::start(std::span<http::request::header::field> fields, http::request::header::field_handler_t handler, void* p_context)
    {
        for (;;) {
            int err = ::listen(m_socket, SOMAXCONN);
            if (err != SOCKET_ERROR) {
                SOCKET client = accept(m_socket, NULL, NULL);

                if (client != INVALID_SOCKET) {
                    auto ret = http::request::header::parse(tcp::stream(client), fields, handler, p_context);
                    std::cout << "======parsed incoming message, sending response======" << std::endl;
                    http::response::header header;
                    http::response::body_buffer body;
                    body << html;
                    if (ret.has_value()) {
                        if (ret->first.method == http::request::method_type::GET && 0 == std::string_view{ ret->first.resource.data() }.compare("/")) {
                            header.print("HTTP/1.1 200 OK\r\n"sv);
                            header.append(http::response::header::field::connection, "Keep-Alive"sv);
                            header.append(http::response::header::field::content_type, "text/html"sv);
                            header.append(http::response::header::field::keep_alive, "timeout=5, max=200"sv);
                            header.end();
                        } else {
                            header.print("HTTP/1.1 404 Not Found\r\n"sv);
                            header.append(http::response::header::field::connection, "close"sv);
                            header.append(http::response::header::field::content_type, "text/html"sv);
                            header.end();
                        }
                    }

                    std::cout << header.data().data() << std::endl;
                    std::cout << body.data().data() << std::endl;

                    int err = send(client, header.data().data(), header.data().size(), 0);
                    if (err > 0)
                        err = send(client, body.data().data(), body.data().size(), 0);
                    else
                        detail::print_last_wsa_error();

                    assert(shutdown(client, SD_BOTH) != SOCKET_ERROR);
                    closesocket(client);
                }

            } else {
                tcp::detail::print_last_wsa_error();
            }
        }

        WSACleanup();

    }

} // namespace http
