#pragma once

#include "encoding/encoding.hpp"
#include "mb/string.hpp"

namespace util {

#if defined(_WIN32)

using sys_string_encoding_type = enc::utf16;
#define SYSSTR(text) u##text

#else

using sys_string_encoding_type = enc::utf8;
#define SYSSTR(text) u8##text

#endif

using sys_string = mb::basic_string<sys_string_encoding_type>;
using sys_string_view = mb::basic_string_view<sys_string_encoding_type>;
using params_list = const std::vector<sys_string_view>&;

extern "C" int exec(params_list params);

    
}