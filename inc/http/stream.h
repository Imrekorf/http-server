/**
 * @file stream.h
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#include "tcp/stream.h"
#include "http/request.h"
#include <array>
#include <string_view>
#include <cstring>

namespace http {
    template<size_t buffer_size>
    class stream;

    template<size_t buffer_size>
    stream<buffer_size>& operator>>(stream<buffer_size>& s, std::string_view& line);
    template<size_t buffer_size>
    stream<buffer_size>& operator>>(stream<buffer_size>& s, request::body& b);

    template<size_t buffer_size>
    class stream {
        tcp::stream& m_ts;
        std::array<char, buffer_size> m_buffer{ '\0' };
        size_t pos_tracker = m_buffer.size() + 1;

    public:

        stream(tcp::stream& ts) : m_ts{ts} { }

        friend stream<buffer_size>& operator>><>(stream<buffer_size>& s, request::body & line);
        friend stream<buffer_size>& operator>><>(stream<buffer_size>& s, std::string_view & b);
    };


    template<size_t buffer_size>
    http::stream<buffer_size>& operator>>(http::stream<buffer_size>& s, std::string_view& line)
    {
        constexpr std::string_view eol{ "\r\n" };

        // first time receive could require a delay
        if (s.m_buffer[0] == '\0' && s.pos_tracker == buffer_size + 1) {
            std::span<char> tcp_writable_area{ s.m_buffer.begin(), s.m_buffer.end() };
            s.m_ts >> tcp_writable_area;
            // retry looking for eol
            s.pos_tracker = 0;
        }

        for (size_t i = 0; i < 2; ++i) {
            std::string_view remainder{ s.m_buffer.begin() + s.pos_tracker, s.m_buffer.end() };

            size_t pos = remainder.find(eol);
            if (pos == std::string_view::npos) {
                if (s.m_ts.available()) {
                    // move data to start of array
                    std::memmove(s.m_buffer.data(), s.m_buffer.data() + s.pos_tracker, remainder.size());
                    std::memset(s.m_buffer.data() + remainder.size(), 0, s.m_buffer.size() - remainder.size());
                    // append new data to end of old data
                    std::span<char> tcp_writable_area{ s.m_buffer.begin() + remainder.size(), s.m_buffer.end() };
                    s.m_ts >> tcp_writable_area;
                    // retry looking for eol
                    s.pos_tracker = 0;
                    continue;
                } else {
                    break;
                }
            } else {
                // move line view to found substring
                line           = remainder.substr(0, pos);
                s.pos_tracker += line.size() + eol.size();
                if (s.pos_tracker > buffer_size)
                    s.pos_tracker = buffer_size;

                return s;
            }
        }

        // error exit
        line          = line.substr(0, 0);
        s.pos_tracker = buffer_size;

        return s;
    }

    template<size_t buffer_size>
    http::stream<buffer_size>& operator>>(http::stream<buffer_size>& s, request::body& b)
    {
        for (;;) {
            std::string_view remainder{ s.m_buffer.begin() + s.pos_tracker, s.m_buffer.end() };

            b << remainder;
            if (s.m_ts.available()) {
                // move data to start of array
                std::memmove(s.m_buffer.data(), s.m_buffer.data() + s.pos_tracker, remainder.size());
                std::memset(s.m_buffer.data() + remainder.size(), 0, s.m_buffer.size() - remainder.size());
                // append new data to end of old data
                std::span<char> tcp_writable_area{ s.m_buffer.begin() + remainder.size(), s.m_buffer.end() };
                s.m_ts >> tcp_writable_area;
                // retry looking for eol
                s.pos_tracker = 0;
                continue;
            } else {
                break;
            }
        }

        return s;
    }

} // namespace http
