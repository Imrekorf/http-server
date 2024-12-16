#include <winerror.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cassert>

#include "tcp/server.h"
#include "tcp/detail.h"
#include "http/request.h"

const char html[] = "HTTP/1.1 200 OK\r\n"
                    "Connection: close\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>\r\n"
                    "<head>\r\n"
                    "<title>Hello, world!</title>\r\n"
                    "</head>\r\n"
                    "<body>\r\n"
                    "<h1>Hello, world!</h1>\r\n"
                    "</body>\r\n"
                    "</html>\r\n\r\n";

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
                    if (ret.has_value()) {
                        
                    }

                    assert(send(client, html, strlen(html) - 1, 0) > 0);
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
