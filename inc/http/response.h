/**
 * @file response.h
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "request.h"
#include <string_view>
#include <cassert>
#include <array>
#include <span>

namespace http::response {

    static constexpr size_t header_buffer_size = 512;
    static constexpr size_t body_buffer_size   = 1024;

    class header {
        std::array<char, header_buffer_size> m_buffer;
        std::span<char> m_content;

    public:

        enum class field {
            age,
            allow,
            connection,
            content_encoding,
            content_language,
            content_length,
            content_location,
            content_range,
            content_type,
            date,
            expires,
            last_modified,
            link,
            location,
            retry_after,
            server,
        };
    private:

        constexpr std::string_view to_string(field f)
        {
            switch (f) {
                case field::age: return "Age";
                case field::allow: return "Allow";
                case field::connection: return "Connection";
                case field::content_encoding: return "Content-Encoding";
                case field::content_language: return "Content-Language";
                case field::content_length: return "Content-Length";
                case field::content_location: return "Content-Location";
                case field::content_range: return "Content-Range";
                case field::content_type: return "Content-Type";
                case field::date: return "Date";
                case field::expires: return "Expires";
                case field::last_modified: return "Last-Modified";
                case field::link: return "Link";
                case field::location: return "Location";
                case field::retry_after: return "Retry-After";
                case field::server: return "Server";
            }

            assert(false);

            return "";
        }

    public:

        std::span<char> data(void);

    };

    class body {
    public:

        virtual std::span<const char> data() = 0;
    };

    class body_buffer : public body {
        std::array<char, body_buffer_size> m_buffer{ '\0' };
        std::span<char> m_content{ m_buffer.begin(), 0 };

    public:

        std::span<const char> data(void);
        size_t                print(std::span<const char> data);

        friend body_buffer& operator<<(body_buffer& b, std::span<const char> data);
    };

    // TODO:
    // class body_file : public body {

    // };

    body_buffer& operator<<(body_buffer& b, std::span<const char> data);

} // namespace http::response
