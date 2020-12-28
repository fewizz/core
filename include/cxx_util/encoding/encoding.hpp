#pragma once

#include "../locale/codecvt.hpp"
#include <corecrt.h>
#include <cwchar>
#include <locale>
#include <stdexcept>
#include <bit>
#include <cstring>
#include <stdint.h>
#include "utf8.hpp"
#include "utf16.hpp"
#include "size_retrieve_result.hpp"

namespace enc {

struct utf8 {
    using char_type = char8_t;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        return util::utf8::first_char_length(begin, end);
    }
};

struct utf16 {
    using char_type = char16_t;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        return util::utf16::first_char_length(begin, end);
    }
};

struct ascii {
    using char_type = char;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        if(begin >= end) throw std::runtime_error{"precondition"};
        return { std::codecvt_base::ok, 1 };
    }
};

struct usc2 {
    using char_type = char16_t;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        if(begin >= end) throw std::runtime_error{"precondition"};
        return { std::codecvt_base::ok, 2 };
    }
};

template<class T>
concept encoding = requires() {
    {T::first_char_length(nullptr, nullptr)} -> std::same_as<size_retrieve_result>;
};

template<class T>
struct is_encoding : std::false_type {}; 

template<encoding T>
struct is_encoding<T> : std::true_type {};

template<class T>
static constexpr bool is_encoding_v = is_encoding<T>::value;

}
