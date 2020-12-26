#pragma once

#include "codecvt.hpp"
#include <cwchar>

namespace util {

namespace enc {

namespace internal {

    template<class CharT, class Codecvt>
    struct codecvt {
        using char_type = CharT;

        static int first_char_length(const char_type* begin, const char_type* end) {
            return util::codecvt<Codecvt>::first_char_length(begin, end);
        }
    };
}

struct utf8 : internal::codecvt<char, std::codecvt_utf8<char16_t>> {};
struct utf16 : internal::codecvt<char16_t, std::codecvt_utf16<char16_t>> {};

struct ascii {
    using char_type = char;

    static int first_char_length(const char_type* begin, const char_type* end) { return 1; }
};

struct usc2 {
    using char_type = char16_t;

    static int first_char_length(const char_type* begin, const char_type* end) { return 2; }
};

template<class Internal, class External>
struct codec;

template<> struct codec<utf16, utf8>
{ using type = std::codecvt_utf8_utf16<char16_t>; };
template<> struct codec<usc2, utf8>
{ using type = std::codecvt_utf8<char16_t>; };
template<> struct codec<utf8, utf8>
{ using type = util::deletable_facet<std::codecvt<char, char, std::mbstate_t>>; };
template<> struct codec<utf8, ascii>
{ using type = util::deletable_facet<std::codecvt<char, char, std::mbstate_t>>; };
template<> struct codec<ascii, ascii>
{ using type = util::deletable_facet<std::codecvt<char, char, std::mbstate_t>>; };

}

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