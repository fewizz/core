#pragma once

#include "codecvt.hpp"
#include <corecrt.h>
#include <cwchar>
#include <locale>
#include <stdexcept>
#include <bit>
#include <cstring>
#include <stdint.h>
#include "utf8.hpp"
#include "utf16.hpp"

namespace enc {

struct utf8 {
    using char_type = char8_t;

    static unsigned first_char_length(const char_type* begin, const char_type* end) {
        return util::utf8::first_char_length(begin, end);
    }
};

namespace internal {

template<std::endian Endian = std::endian::big>
struct _utf16 {
    using char_type = char16_t;

    static int first_char_length(const char_type* begin, const char_type* end) {
        return util::utf16::first_char_length(Endian, begin, end);
    }
};

}

using utf16be = internal::_utf16<std::endian::big>;
using utf16le = internal::_utf16<std::endian::little>;
using utf16 = utf16be;

struct ascii {
    using char_type = char;

    static int first_char_length(const char_type* begin, const char_type* end) { return 1; }
};

struct usc2 {
    using char_type = char16_t;

    static int first_char_length(const char_type* begin, const char_type* end) { return 2; }
};

template<class T>
concept encoding = requires() {
    {T::first_char_length(nullptr, nullptr)} -> std::integral;
};

template<class T>
struct is_encoding : std::false_type {}; 

template<encoding T>
struct is_encoding<T> : std::true_type {};

template<class T>
static constexpr bool is_encoding_v = is_encoding<T>::value;

}
