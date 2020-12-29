#pragma once

#include <bit>
#include <locale>
#include <stdexcept>
#include <stdint.h>
#include <utility>
#include <codecvt>
#include "codepoint_request_result.hpp"

namespace util {

namespace utf16 {

inline enc::size_request_info first_char_width(const char16_t* begin, const char16_t* end) {
    auto size = end - begin;

    if(size < 0) return { enc::request_result::preconditions };
    if(size == 0) return { enc::request_result::unexpected_src_end };

    uint16_t first = begin[0];

    if(first >= 0x0 && first <= 0xD800 || first >= 0xE000 && first <= 0xFFFF)
        return { enc::request_result::ok, 1 };
    
    if(size == 1) { return { enc::request_result::unexpected_src_end }; }

    uint16_t second = begin[1];

    uint16_t hs = first, ls = second;

    if(hs >= 0xD800 && hs <= 0xDBFF && ls >= 0xDC00 && ls <= 0xDFFF)
        return { enc::request_result::ok, 2 };
    
    return { enc::request_result::invalid_input };
}

inline enc::codepoint_request_info first_code_point(const char16_t* begin, const char16_t* end) noexcept {
    auto size = end - begin;

    if(size < 0) return { enc::request_result::preconditions };
    if(size == 0) return { enc::request_result::unexpected_src_end };

    auto size_read = first_char_width(begin, end);
    if(size_read.result != enc::request_result::ok) return { size_read.result };

    if(size_read.size == 1) return { enc::request_result::ok, begin[0], 1 };

    uint64_t hs = begin[0], ls = begin[1];

    return {
        enc::request_result::ok,
        (((hs & 0x3FF) << 10) | (ls & 0x3FF)) + 0x10000,
        2
    };
}

}

}