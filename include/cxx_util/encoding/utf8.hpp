#pragma once

#include <bits/c++config.h>
#include <corecrt.h>
#include <cstdint>
#include <iterator>
#include <locale>
#include <stdint.h>
#include <type_traits>
#include <utility>
#include <cinttypes>
#include <stdexcept>
#include <codecvt>
#include "request_error.hpp"
#include "../int.hpp"
#include "../bit.hpp"
#include <array>

#include <tl/expected.hpp>

namespace util {

namespace utf8 {

constexpr tl::expected<uint8_t, enc::request_error> possible_char_width(std::byte byte) {
    if(equalsl<0>(byte)) return { 1 };
    if(equalsl<1, 1, 0>(byte)) return { 2 };
    if(equalsl<1, 1, 1, 0>(byte)) return { 3 };
    if(equalsl<1, 1, 1, 1, 0>(byte)) return { 4 };
    return tl::unexpected{ enc::request_error::invalid_input };
}

namespace internal {

template<class... Ints>
constexpr tl::expected<uint8_t, enc::request_error>
first_char_width(Ints... ints/*std::array<uint8_t, N> arr*/) {
    using Common = std::common_type_t<Ints...>;
    constexpr uint8_t N = sizeof...(Ints);
    std::array<Common, sizeof...(Ints)> arr{ ints... };

    auto possible = possible_char_width(arr[0]);

    if(!possible) return possible;

    for(unsigned i = 1; i < std::min(possible.value(), N); i++)
        if(not equalsl<1,0>(arr[i])) return tl::unexpected{ enc::request_error::invalid_input };

    if(possible.value() > N) return tl::unexpected{ enc::request_error::unexpected_src_end };

    return possible;
}

}

constexpr tl::expected<uint8_t, enc::request_error> first_char_width(std::byte b0, std::byte b1, std::byte b2, std::byte b3) {
    return internal::first_char_width(b0, b1, b2, b3);
}

constexpr tl::expected<uint8_t, enc::request_error> first_char_width(std::byte b0, std::byte b1, std::byte b2) {
    return internal::first_char_width(b0, b1, b2);
}

constexpr tl::expected<uint8_t, enc::request_error> first_char_width(std::byte b0, std::byte b1) {
    return internal::first_char_width(b0, b1);
}

constexpr tl::expected<uint8_t, enc::request_error> first_char_width(std::byte b0) {
    return internal::first_char_width(b0);
}

template<std::ranges::range R>
constexpr tl::expected<uint8_t, enc::request_error> first_char_width(const R& bytes) {
    auto size = std::size(bytes);
    auto it = std::begin(bytes);

    if(size >= 4) return first_char_width(*it, *(it+1), *(it+2), *(it+3));
    else if(size == 3) return first_char_width(*it, *(it+1), *(it+2));
    else if(size == 2) return first_char_width(*it, *(it+1));
    else return first_char_width(*it);
}

/*template<uint8_t N>
constexpr tl::expected<class T, class E> first_code_point() {
    constexpr uint8_t size = sizeof...(Ints);
    uint8_t arr[size] { Ints... };

    enc::size_request_info width_request_result = first_char_width<Ints...>();
    if(width_request_result.result != enc::request_result::ok) return { width_request_result.result };

    uint64_t first = arr[0];

    if(width == 1) return first;

    auto left_mask_size = width + 1;
    first &= (0xFF >> left_mask_size);

    unsigned first_offset = (6 * (width - 1));
    uint64_t result = first << first_offset;

    for(unsigned i = 1; i < (unsigned)width; i++) {
        unsigned offset = 6 * ((width - 1) - i);
        uint64_t nth = arr[i];
        result |= (nth & 0b00111111) << offset;
    }

    return result;
}

inline enc::size_request_info first_char_width(const uint8_t* begin, const uint8_t* end) noexcept {
    auto ptr_diff = end - begin;
    if(ptr_diff < 0) return { enc::request_result::preconditions };

    unsigned size = ptr_diff;
    if(size == 0) return { enc::request_result::unexpected_src_end };

    uint8_t first = begin[0];

    unsigned potential_size = possible_char_width(first);
    
    if(potential_size == 0) return { enc::request_result::invalid_input };

    for(unsigned i = 1; i < std::min(potential_size, size); i++) {
        if(not starts_with(0b10, 2, begin[i])) return { enc::request_result::invalid_input };
    }

    if(potential_size > size) return { enc::request_result::unexpected_src_end };

    return { enc::request_result::ok, potential_size };
}

inline enc::codepoint_request_info first_code_point(const uint8_t* begin, const uint8_t* end) noexcept {
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
enc::codepoint_request_info first_code_point(const uint8_t (& begin)[N]) noexcept {
    return first_code_point(begin, begin + N);
}*/

}

}