#pragma once

#include "encoding/encoding.hpp"
#include "mb/string.hpp"
#include "mb/string_view.hpp"
#include <vector>

namespace util {

#if defined(_WIN32) && defined(UNICODE)

using sys_string_encoding_type = enc::utf16;
#define SYSSTR(text) u##text

#else

using sys_string_encoding_type = enc::utf8;
#define SYSSTR(text) u8##text

#endif

using sys_string = mb::basic_string<sys_string_encoding_type>;
using sys_string_view = mb::basic_string_view<sys_string_encoding_type>;
    
}