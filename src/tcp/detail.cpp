/**
 * @file detail.cpp
 * @author Imre Korf (I.korf@outlook.com)
 * @date 2024-12-16
 *
 * @copyright Copyright (c) 2024
 */

#include "tcp/detail.h"
#include <iostream>
#include <winerror.h>
#include <windows.h>

namespace tcp::detail {
    void print_last_wsa_error(void)
    {
        wchar_t* s = NULL;
        FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, WSAGetLastError(),
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPWSTR)&s, 0, NULL);
        std::wcerr << s << std::endl;
        LocalFree(s);
    }

} // namespace tcp::detail
