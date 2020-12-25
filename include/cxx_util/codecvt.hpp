#pragma once

#include <codecvt>
#include <locale>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "facet.hpp"

namespace util {

template<class Codecvt>
struct codecvt {
    
using extern_type = typename Codecvt::extern_type;
using intern_type = typename Codecvt::intern_type;
using state_type = typename Codecvt::state_type;

static inline unsigned first_char_length(
    const extern_type* begin,
    const extern_type* end
) {
    Codecvt codec{};
    state_type state{};

    return codec.length(state, begin, end, 1);
}

static inline unsigned converted_length(
    const extern_type* begin,
    const extern_type* end
) {
    Codecvt codec{};
    state_type state{};

    intern_type temp;
    intern_type* temp_ptr;

    unsigned result = 0;

    while(true) {
        const extern_type* begin_next;
        begin += codec.length(state, begin, end, 1);
        result++;

        if(begin >= end) return result;
    }
}

static inline std::basic_string<intern_type> convert_in(
    const extern_type* begin,
    const extern_type* end
) {
    Codecvt codec{};

    if(codec.always_noconv()) return {begin, end};

    std::basic_string<intern_type> result(
        converted_length(begin, end),
        0
    );

    state_type state{};

    const extern_type* temp_ext;
    intern_type* temp_int;

    auto res = codec.in(state, begin, end, temp_ext, result.data(), result.data() + result.size(), temp_int);

    if(res == std::codecvt_base::noconv) {
        return {begin, end};
    }
    if(res != std::codecvt_base::ok)
        throw std::runtime_error{
            "convert_mbc: codec.in. result: " + std::to_string(res) +
            ", size: "+ std::to_string(end - begin) +
            ", result size: " + std::to_string(result.size())
        };

    return result;
}

template<class It>
static std::basic_string_view<extern_type> next_mbc(It& begin, It end) {
    int l = first_char_length(&*begin, &*end);
    begin += l;
    return {begin - l, begin};
}

};

}