/**
 * @file request.h
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string_view>
#include <array>
#include <span>
#include <optional>
#include <stdint.h>
#include <cassert>

#include "tcp/stream.h"


namespace http::request {

    constexpr size_t max_body_size        = 1500;
    constexpr size_t tcp_max_read_size    = 512;
    constexpr size_t resource_buffer_size = 64;

    class body;

    enum class method_type {
        GET,
        PUT,
        POST,
        DEL,
    };

    struct header_info {
        method_type                            method = method_type::GET;
        std::array<char, resource_buffer_size> resource{ '\0' };
    };

    class header {
    public:

        enum class field {
            accept,
            accept_charset,
            accept_datetime,
            accept_encoding,
            accept_language,
            authorization,
            connection,
            content_encoding,
            content_length,
            content_type,
            date,
            expect,
            forwarded,
            from,
            host,
            user_agent,
            max_forwards,
            origin,
        };

        typedef void (*field_handler_t)(field f, std::string_view value, void* p_context);

#ifdef NDEBUG
    private:
#endif

        static constexpr std::string_view to_string(field f)
        {
            switch (f) {
                case field::accept: return "Accept";
                case field::accept_charset: return "Accept-Charset";
                case field::accept_datetime: return "Accept-Datetime";
                case field::accept_encoding: return "Accept-Encoding";
                case field::accept_language: return "Accept-Language";
                case field::authorization: return "Authorization";
                case field::connection: return "Connection";
                case field::content_encoding: return "Content-Encoding";
                case field::content_length: return "Content-Length";
                case field::content_type: return "Content-Type";
                case field::date: return "Date";
                case field::expect: return "Expect";
                case field::forwarded: return "Forwarded";
                case field::from: return "From";
                case field::host: return "Host";
                case field::user_agent: return "User-Agent";
                case field::max_forwards: return "Max-Forwards";
                case field::origin: return "Origin";
            }

            assert(false);

            return "";
        }

        static constexpr std::string_view to_string(method_type m)
        {
            switch (m) {
                case method_type::GET: return "GET";
                case method_type::PUT: return "PUT";
                case method_type::DEL: return "DELETE";
                case method_type::POST: return "POST";
            }

            assert(false);

            return "";
        }

    public:

        static std::optional<std::pair<header_info, body>> parse(tcp::stream req, std::span<field> fields, field_handler_t handler, void* p_context);
    };

    class body {
        std::array<char, max_body_size> m_body_buffer{ '\0' };
        std::span<char> m_body_contents{ m_body_buffer.begin(), 0 };
        void* m_p_context;

        body(void* p_context);

        friend body& operator<<(body& b, std::string_view data);
        friend header;
    };
}; // namespace http::request
