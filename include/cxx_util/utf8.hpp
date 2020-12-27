#pragma once

#include <stdint.h>
#include <utility>
#include <cinttypes>
#include <stdexcept>

namespace util {

namespace utf8 {

inline unsigned first_char_length(const char8_t* begin, const char8_t* end) {
    int first = int(*begin);
    if((first >> 7) == 0) return 1;
    if((first >> 5) == 0b110) return 2;
    if((first >> 4) == 0b1110) return 3;
    if((first >> 3) == 0b11110) return 4;

    throw std::runtime_error{"invalid utf-8 input"};
}

inline std::pair<uint64_t, unsigned> first_code_point(const char8_t* begin, const char8_t* end) {
    int length = first_char_length(begin, end);

    if(length == 1) return { begin[0], 1 };

    int first_shift;
    int first_mask;

    if(length == 2) { first_shift = 6;  first_mask = 0b00011111; }
    if(length == 3) { first_shift = 12; first_mask = 0b00001111; } 
    if(length == 4) { first_shift = 18; first_mask = 0b00000111; } 

    uint64_t result = (begin[0] & first_mask) << first_shift;

    int second_shift;

    if(length == 2) second_shift = 0;
    if(length == 3) second_shift = 6;
    if(length == 4) second_shift = 12;

    result |= (begin[1] & 0b00111111) << second_shift;
    if(length == 2) return { result, 2 };

    int third_shift = length == 3 ? 0 : 6;
    result |= (begin[2] & 0b00111111) << third_shift;
    if(length == 3) return { result, 3 };

    return { result | (begin[3] & 0b00111111), 4 };
}

}

}