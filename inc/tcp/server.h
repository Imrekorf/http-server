/**
 * @file server.h
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#pragma once

#include <string_view>
#include <chrono>

#include "http/request.h"

namespace tcp {
    class server {


        unsigned long long m_socket;
        static constexpr std::chrono::seconds max_keep_alive{ 2 }; // set to 0 to disable keep_alive

    public:

        server(std::string_view port);

        void start(std::span<http::request::header::field> fields, http::request::header::field_handler_t handler, void* p_context);
    };

} // namespace http
