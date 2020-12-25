#pragma once

#include "mbc.hpp"
#include <codecvt>
#include <cwchar>

namespace util {

template<class CharT, class Codecvt>
struct codecvt_encoding {
    using char_type = CharT;

    static int first_char_length(auto begin, auto end) {
        return first_mbc_length<Codecvt>(begin, end);
    }
};

using utf8_encoding = codecvt_encoding<char, std::codecvt_utf8<char16_t>>;
using utf16_encoding = codecvt_encoding<char16_t, std::codecvt_utf16<char16_t>>;

struct ascii_encoding {
    using char_type = char;

    static int first_char_length(auto begin, auto end) { return 1; }
};

struct usc2_encoding {
    using char_type = char16_t;

    static int first_char_length(auto begin, auto end) { return 2; }
};

template<class Internal, class External>
struct codec;

template<> struct codec<utf16_encoding, utf8_encoding>
{ using type = std::codecvt_utf8_utf16<char16_t>; };
template<> struct codec<usc2_encoding, utf8_encoding>
{ using type = std::codecvt_utf8<char16_t>; };
template<> struct codec<utf8_encoding, ascii_encoding>
{ using type = std::codecvt<char, char, std::mbstate_t>; };
template<> struct codec<ascii_encoding, ascii_encoding>
{ using type = util::deletable_facet<std::codecvt<char, char, std::mbstate_t>>; };

}