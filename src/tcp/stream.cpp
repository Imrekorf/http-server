/**
 * @file tcpstream.cpp
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#include "tcp/stream.h"
#include "tcp/detail.h"
#include <winsock.h>

namespace tcp {
    stream& operator>>(stream& is, std::span<char>& buffer)
    {
        int ret = ::recv(is.m_client, buffer.data(), buffer.size(), 0);
        if (SOCKET_ERROR == ret) {
            tcp::detail::print_last_wsa_error();
            buffer = buffer.first(0);
        } else {
            buffer = buffer.first(ret);
        }

        return is;
    }

    size_t stream::available(void)
    {
        unsigned long bytes_available = 0;
        if (0 == ioctlsocket(m_client, FIONREAD, &bytes_available)) {
            return bytes_available;
        } else {
            tcp::detail::print_last_wsa_error();

            return 0;
        }
    }

} // namespace tcp
