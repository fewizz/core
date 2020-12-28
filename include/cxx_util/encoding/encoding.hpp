#pragma once

#include <stdexcept>
#include <stdint.h>
#include "utf8.hpp"
#include "utf16.hpp"
#include "size_retrieve_result.hpp"

namespace enc {

static constexpr int variable_length = -1;

struct utf8 {
    using char_type = char8_t;
    static constexpr int characters = variable_length;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        return util::utf8::first_char_length(begin, end);
    }
};

struct utf16 {
    using char_type = char16_t;
    static constexpr int characters = variable_length;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        return util::utf16::first_char_length(begin, end);
    }
};

struct ascii {
    using char_type = char;
    static constexpr int characters = 1;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        if(begin >= end) throw std::runtime_error{ "precondition" };
        return { std::codecvt_base::ok, 1 };
    }
};

struct usc2 {
    using char_type = char16_t;
    static constexpr int characters = 2;

    static size_retrieve_result first_char_length(const char_type* begin, const char_type* end) {
        if(begin >= end) throw std::runtime_error{ "precondition" };
        return { std::codecvt_base::ok, 2 };
    }
};

template<class T>
concept encoding = requires() {
    {T::first_char_length(nullptr, nullptr)} -> std::same_as<size_retrieve_result>;
    {T::characters} -> std::same_as<const int>;
};

template<class T>
struct is_encoding : std::false_type {}; 

template<encoding T>
struct is_encoding<T> : std::true_type {};

template<class T>
static constexpr bool is_encoding_v = is_encoding<T>::value;

}
