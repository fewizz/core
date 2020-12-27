#pragma once

#include <locale>
#include <stdint.h>
#include <utility>
#include <cinttypes>
#include <stdexcept>
#include <codecvt>
#include "size_retrieve_result.hpp"
#include "codepoint_retrieve_result.hpp"

namespace util {

namespace utf8 {

inline enc::size_retrieve_result first_char_length(const char8_t* begin, const char8_t* end) noexcept {
    auto size = end - begin;

    if(size < 0) return { std::codecvt_base::error };
    if(size == 0) return { std::codecvt_base::partial };

    int first = begin[0];

    if((first >> 7) == 0) return { std::codecvt_base::ok, 1 };

    if((first >> 6) != 0b11) return { std::codecvt_base::error };

    if(size == 1) return { std::codecvt_base::partial };

    if((first >> 5) == 0b110) return { std::codecvt_base::ok, 2 };

    if(size == 2) return { std::codecvt_base::partial };

    if((first >> 4) == 0b1110) return { std::codecvt_base::ok, 3};

    if(size == 3) return { std::codecvt_base::partial };

    if((first >> 3) == 0b11110) return { std::codecvt_base::ok, 4};

    return { std::codecvt_base::error };
}

struct codepoint_retrieve_result {
    std::codecvt_base::result result;
    uint64_t codepoint;
    unsigned size_result;
};

inline enc::codepoint_retrieve_result first_code_point(const char8_t* begin, const char8_t* end) {
    auto size = end - begin;

    if(size < 0) return { std::codecvt_base::error };
    if(size == 0) return { std::codecvt_base::partial };

    auto size_read = first_char_length(begin, end);
    if(size_read.result != std::codecvt_base::ok) return { size_read.result };

    if(size_read.size == 1) return { std::codecvt_base::ok, begin[0], 1 };

    int first_shift;
    int first_mask;

    if(size_read.size == 2) { first_shift = 6;  first_mask = 0b00011111; }
    if(size_read.size == 3) { first_shift = 12; first_mask = 0b00001111; } 
    if(size_read.size == 4) { first_shift = 18; first_mask = 0b00000111; } 

    uint64_t result = (begin[0] & first_mask) << first_shift;

    int second_shift;

    if(size_read.size == 2) second_shift = 0;
    if(size_read.size == 3) second_shift = 6;
    if(size_read.size == 4) second_shift = 12;

    result |= (begin[1] & 0b00111111) << second_shift;
    if(size_read.size == 2) return { std::codecvt_base::ok, result, 2 };

    int third_shift = size_read.size == 3 ? 0 : 6;
    result |= (begin[2] & 0b00111111) << third_shift;
    if(size_read.size == 3) return { std::codecvt_base::ok, result, 3 };

    return { std::codecvt_base::ok, result | (begin[3] & 0b00111111), 4 };

}

template<unsigned N>
enc::codepoint_retrieve_result first_code_point(const char8_t (& begin)[N]) {
    return first_code_point(begin, begin + N);
}

}

}