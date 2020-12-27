#pragma once

#include <bits/c++config.h>
#include <codecvt>
#include <cwchar>
#include <locale>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <wchar.h>
#include "facet.hpp"
#include <iostream>
#include <bit>

#include "utf8.hpp"
#include "utf16.hpp"

namespace util {

template<class Codecvt>
struct codecvt {
    
using extern_type = typename Codecvt::extern_type;
using intern_type = typename Codecvt::intern_type;
using state_type = typename Codecvt::state_type;

static inline unsigned from_external_to_internal_length(
    const extern_type* begin,
    const extern_type* end
) {
    Codecvt codec{};
    state_type state{};

    intern_type to[0x100];
    intern_type* to_next;

    unsigned result = 0;

    while(true) {
        auto res = codec.in(state, begin, end, begin, to, to + std::extent_v<decltype(to)>, to_next);

        if(res == std::codecvt_base::ok)
            result += to_next - to;
        else if(res == std::codecvt_base::partial)
            throw std::runtime_error{"local buffer is too small"};

        if(begin == end) return result;
    }
}

static inline unsigned from_internal_to_external_length(
    const intern_type* begin,
    const intern_type* end
) {
    Codecvt codec{};
    state_type state{};

    extern_type to[0x100];
    extern_type* to_next;

    unsigned result = 0;

    while(true) {
        auto res = codec.out(state, begin, end, begin, to, to + std::extent_v<decltype(to)>, to_next);

        if(res == std::codecvt_base::ok)
            result += to_next - to;
        else if(res == std::codecvt_base::partial)
            throw std::runtime_error{"local buffer is too small"};

        if(begin == end) return result;
    }
}

};

}