/**
 * @file response.cpp
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#include "http/response.h"
#include <cstring>

using std::string_view_literals::operator""sv;

namespace http::response {
    size_t header::append(field f, std::span<const char> value)
    {
        size_t old_size = m_content.size();
        *this << to_string(f) << ": "sv << value << "\r\n"sv;

        return m_content.size() - old_size;
    }

    size_t header::end(void)
    {
        size_t old_size = m_content.size();
        *this << "\r\n"sv;

        return m_content.size() - old_size;
    }

} // namespace http::response
