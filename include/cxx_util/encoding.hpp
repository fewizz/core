#pragma once

#include "mbc.hpp"
#include <codecvt>

namespace util {

template<class Codecvt>
struct codecvt_encoding {
    using char_type = typename Codecvt::extern_type;

    static int first_char_length(auto begin, auto end) {
        return first_mbc_length<Codecvt>(begin, end);
    }
};

using utf8_encoding = codecvt_encoding<std::codecvt_utf8<char16_t>>;
using utf16_encoding = codecvt_encoding<std::codecvt_utf16<char16_t>>;

template<class From, class To>
struct codec;

template<>
struct codec<utf8_encoding, utf16_encoding> { using type = std::codecvt_utf8_utf16<char16_t>; };

}