#pragma once

#include <corecrt.h>
#include <locale>
#include <stdint.h>
#include <utility>
#include <cinttypes>
#include <stdexcept>
#include <codecvt>
#include "codepoint_request_result.hpp"

namespace util {

namespace utf8 {

inline enc::size_request_info first_char_width(const char8_t* begin, const char8_t* end) noexcept {
    auto ptr_diff = end - begin;
    if(ptr_diff < 0) return { enc::request_result::preconditions };

    unsigned size = ptr_diff;
    if(size == 0) return { enc::request_result::unexpected_src_end };

    uint8_t first = begin[0];

    auto starts_with = [](uint8_t mask, int bits, uint8_t val){
        return (val >> (8 - bits)) == mask;
    };

    unsigned potential_size = 0;

    if(starts_with(0b0, 1, first))
        potential_size = 1;
    if(starts_with(0b110, 3, first))
        potential_size = 2;
    if(starts_with(0b1110, 4, first))
        potential_size = 3;
    if(starts_with(0b11110, 5, first))
        potential_size = 4;
    
    if(potential_size == 0) return { enc::request_result::invalid_input };

    for(unsigned i = 1; i < std::min(potential_size, size); i++) {
        if(not starts_with(0b10, 2, begin[i])) return { enc::request_result::invalid_input };
    }

    if(potential_size > size) return { enc::request_result::unexpected_src_end };

    return { enc::request_result::ok, potential_size };
}

inline enc::codepoint_request_info first_code_point(const char8_t* begin, const char8_t* end) noexcept {
    auto size = end - begin;

    if(size < 0) return { enc::request_result::preconditions };
    if(size == 0) return { enc::request_result::unexpected_src_end };

    auto size_read = first_char_width(begin, end);
    if(size_read.result != enc::request_result::ok) return { size_read.result };

    uint64_t first = begin[0];

    if(size_read.size == 1) return { enc::request_result::ok, first, 1 };

    unsigned left_mask_size = size_read.size + 1;
    first &= (0xFF >> left_mask_size);
    unsigned first_offset = (6 * (size_read.size - 1));
    uint64_t result = first << first_offset;

    for(unsigned i = 1; i < (unsigned)size_read.size; i++) {
        unsigned offset = 6 * ((size_read.size - 1) - i);
        uint64_t nth = begin[i];
        result |= (nth & 0b00111111) << offset;
    }

    return { enc::request_result::ok, result, 4 };
}

template<unsigned N>
enc::codepoint_request_info first_code_point(const char8_t (& begin)[N]) noexcept{
    return first_code_point(begin, begin + N);
}

}

}