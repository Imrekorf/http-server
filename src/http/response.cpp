/**
 * @file response.cpp
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#include "http/response.h"
#include <cstring>

namespace http::response {
    std::span<const char> body_buffer::data(void) { return m_content; }

    size_t body_buffer::print(std::span<const char> data)
    {
        size_t avail  = m_buffer.size() - m_content.size();
        size_t nwrite = data.size() > avail ? avail : data.size();
        std::memcpy(m_buffer.begin() + m_content.size(), data.data(), nwrite);

        return nwrite;
    }

    body_buffer& operator<<(body_buffer& b, std::span<const char> data)
    {
        b.print(data);

        return b;
    }

} // namespace http::response
