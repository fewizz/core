#pragma once

#include <iterator>
#include <stdexcept>
#include <stdint.h>
#include "utf8.hpp"
#include "utf16.hpp"
#include "codepoint_request_result.hpp"

namespace enc {

static constexpr int variable_width = -1;

struct utf8 {
    using char_type = char8_t;
    static constexpr int characters = variable_width;

    static size_request_info first_char_width(const char_type* begin, const char_type* end) {
        return util::utf8::first_char_width(begin, end);
    }

    static codepoint_request_info first_codepoint(const char_type* begin, const char_type* end) {
        return util::utf8::first_code_point(begin, end);
    }
};

struct utf16 {
    using char_type = char16_t;
    static constexpr int characters = variable_width;

    static size_request_info first_char_width(const char_type* begin, const char_type* end) {
        return util::utf16::first_char_width(begin, end);
    }

    static codepoint_request_info first_codepoint(const char_type* begin, const char_type* end) {
        return util::utf16::first_code_point(begin, end);
    }
};

struct ascii {
    using char_type = char;
    static constexpr int characters = 1;

    static size_request_info first_char_width(const char_type* begin, const char_type* end) {
        if(begin > end) return { enc::request_result::preconditions };
        if(std::distance(begin, end) < 1) return { enc::request_result::unexpected_src_end };
        return { enc::request_result::ok, 1 };
    }

    static codepoint_request_info first_codepoint(const char_type* begin, const char_type* end) {
        if(begin > end) return { enc::request_result::preconditions };
        if(std::distance(begin, end) < 1) return { enc::request_result::unexpected_src_end };
        return { enc::request_result::ok, (uint64_t) begin[0] };
    }
};

struct usc2 {
    using char_type = char16_t;
    static constexpr int characters = 2;

    static size_request_info first_char_width(const char_type* begin, const char_type* end) {
        if(begin > end) return { enc::request_result::preconditions };
        if(std::distance(begin, end) < 2) return { enc::request_result::unexpected_src_end };
        return { enc::request_result::ok, 2 };
    }

    static codepoint_request_info first_codepoint(const char_type* begin, const char_type* end) {
        if(begin > end) return { enc::request_result::preconditions };
        if(std::distance(begin, end) < 1) return { enc::request_result::unexpected_src_end };
        return { enc::request_result::ok, (uint64_t) begin[0] };
    }
};

template<class T>
concept encoding = requires() {
    {T::first_char_width(nullptr, nullptr)};
    {T::first_codepoint(nullptr, nullptr)};
    T::characters;
};

template<class T, class CharT>
concept encoding_with_size_of = encoding<T> && sizeof(typename T::char_type) == sizeof(CharT);

template<class T>
struct is_encoding : std::false_type {}; 

template<encoding T>
struct is_encoding<T> : std::true_type {};

template<class T>
static constexpr bool is_encoding_v = is_encoding<T>::value;

}
