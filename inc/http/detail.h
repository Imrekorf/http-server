/**
 * @file detail.h
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <array>
#include <cstring>
#include <span>

namespace http::detail {
    template<size_t buffer_size>
    class buffer_writer;

    template<size_t buffer_size>
    buffer_writer<buffer_size>& operator<<(buffer_writer<buffer_size>& b, std::span<const char> data);

    template<size_t buffer_size>
    class buffer_writer {
    protected:

        std::array<char, buffer_size> m_buffer{ '\0' };
        std::span<char> m_content{ m_buffer.begin(), 0 };
    public:

        std::span<char> data(void) { return m_content; }
        size_t          print(std::span<const char> data)
        {
            size_t avail  = m_buffer.size() - m_content.size();
            size_t nwrite = data.size() > avail ? avail : data.size();
            std::memcpy(m_buffer.begin() + m_content.size(), data.data(), nwrite);
            m_content = std::span<char>{ m_buffer.begin(), m_content.size() + nwrite };

            return nwrite;
        }

        friend buffer_writer<buffer_size>& operator<<<>(buffer_writer<buffer_size>& b, std::span<const char> data);
    };

    template<size_t buffer_size>
    buffer_writer<buffer_size>& operator<<(buffer_writer<buffer_size>& b, std::span<const char> data)
    {
        b.print(data);

        return b;
    }

} // namespace http::detail
