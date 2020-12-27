#pragma once

#include <bit>
#include <stdexcept>
#include <stdint.h>
#include <utility>

namespace util {

namespace utf16 {

inline unsigned first_char_length(std::endian e, const char16_t* begin, const char16_t* end) {
    uint16_t first = begin[0];
    first >>= 10;
    if(e == std::endian::big && first == 0b110110) return 2;
    if(e == std::endian::little && first == 0b110111) return 2;
    return 1;
}

inline std::pair<uint32_t, unsigned> first_code_point(std::endian e, const char16_t* begin, const char16_t* end) {
    int length = first_char_length(e, begin, end);

    if(length == 1) return { begin[0], 1 };

    uint32_t first = begin[0] & 0b0000001111111111, second = begin[1] & 0b0000001111111111;

    return {
        (e == std::endian::big ? ((first << 10) | second) : ((second << 10) | first)) + 0x10000,
        2
    };
}

}

}