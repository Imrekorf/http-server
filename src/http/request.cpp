/**
 * @file request.cpp
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#include "http/request.h"
#include "http/stream.h"
#include <algorithm>
#include <optional>
#include <string_view>
#include <iostream>

namespace http::request {
    std::optional<std::pair<header_info, body>> header::parse(tcp::stream req, std::span<field> fields, field_handler_t handler, void* p_context)
    {
        http::stream<tcp_max_read_size> http_stream(req);

        header_info h_info{ };

        std::string_view line;
        http_stream >> line;
        if (line.size() < sizeof("GET / HTTP1.x") - 1) {
            std::cout << "message too short!" << std::endl;

            return std::nullopt;
        }

        if (line.starts_with(to_string(method_type::GET)))
            h_info.method = method_type::GET;
        else if (line.starts_with(to_string(method_type::PUT)))
            h_info.method = method_type::PUT;
        else if (line.starts_with(to_string(method_type::DEL)))
            h_info.method = method_type::DEL;
        else if (line.starts_with(to_string(method_type::POST)))
            h_info.method = method_type::POST;
        else {
            std::cout << "unsupported method!" << std::endl;

            return std::nullopt;
        }

        // acquire resource
        std::string_view resource = line.substr(to_string(h_info.method).size() + 1);
        size_t http_version_start = resource.find("HTTP/1.");
        if (http_version_start == std::string_view::npos) {
            std::cout << "invalid HTTP message or unsupported HTTP version!" << std::endl;

            return std::nullopt;
        }

        size_t resource_len = http_version_start - 1;
        std::ranges::copy(resource.substr(0, resource_len > resource_buffer_size ? resource_buffer_size : resource_len), h_info.resource.begin());

        for (http_stream >> line; line.size(); http_stream >> line) {
            if (line.size() == 0)
                break;

            for (auto f : fields) {
                auto field_str = to_string(f);
                if (line.starts_with(field_str)) {
                    handler(f, line.substr(field_str.size() + sizeof(": ") - 1), p_context);
                    break;
                }
            }
        }

        body b{ p_context };
        http_stream >> b;

        return std::pair{
            h_info, b
        };
    }

    body::body(void* p_context) :
      m_p_context(p_context)
    {

    }

    body& operator<<(body& b, std::string_view data)
    {
        size_t copy = b.m_body_buffer.size() - b.m_body_contents.size();
        copy = copy < data.size() ? copy : data.size();
        if (copy == 0) {
            std::cout << "request body buffer too small!" << std::endl;
        }

        std::memcpy(b.m_body_buffer.data() + b.m_body_contents.size(), data.data(), copy);
        b.m_body_contents = std::span<char>{ b.m_body_buffer.begin(), b.m_body_contents.size() + copy };

        return b;
    }

} // namespace http::request
