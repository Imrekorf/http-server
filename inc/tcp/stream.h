/**
 * @file stream.h
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <span>

namespace tcp {

    class stream {
        unsigned long long m_client;
    public:

        stream(unsigned long long client) : m_client{client} { }
        size_t available(void);

        friend stream& operator>>(stream& is, std::span<char>& buffer);
    };

} // namespace tcp
