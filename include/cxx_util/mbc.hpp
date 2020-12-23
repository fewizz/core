#pragma once

#include <codecvt>
#include <locale>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "facet.hpp"

namespace util {

template<class Codecvt>
using ext_t = typename Codecvt::extern_type;

template<class Codecvt>
using int_t = typename Codecvt::intern_type;

template<class Codecvt>
using state_t = typename Codecvt::state_type;

template<class Codecvt>
unsigned first_mbc_length(
    const ext_t<Codecvt>* from,
    const ext_t<Codecvt>* end
) {
    Codecvt codec{};
    state_t<Codecvt> state{};

    return codec.length(state, from, end, 1);
}

template<class Codecvt>
unsigned first_converted_mbc_length(
    const ext_t<Codecvt>* from,
    const ext_t<Codecvt>* end
) {
    int l = first_mbc_length<Codecvt>(from, end);

    Codecvt codec{};
    state_t<Codecvt> state{};

    int_t<Codecvt> temp;
    int_t<Codecvt>* temp_ptr;

    const ext_t<Codecvt>* from_next;

    int length = 0;
    while(true) {
        auto result = codec.in(state, from, end, from_next, &temp, (&temp) + 1, temp_ptr);
        length++;

        int remain = end - from_next;

        if(remain > 0 || result == std::codecvt_base::partial) continue;
        if(result == std::codecvt_base::ok) return length;
        else throw std::runtime_error{"codecvt.in"};
    }
}

template<class Codecvt, class It>
std::basic_string_view<ext_t<Codecvt>> next_mbc(It& begin, It end) {
    int l = first_mbc_length<Codecvt>(&*begin, &*end);
    begin += l;
    return {begin - l, end};
}

template<class Codecvt, class It>
std::basic_string_view<ext_t<Codecvt>> next_converted_mbc(It& begin, It end) {
    int l = first_converted_length<Codecvt>(&*begin, &*end);
    begin += l;
    
    return {begin - l, end};
}

template<class Codecvt>
bool mbc_equals(
    const int_t<Codecvt>* in_from,
    const int_t<Codecvt>* in_end,
    const ext_t<Codecvt>* ex_from,
    const ext_t<Codecvt>* ex_end
) {
    int l = first_mbc_size<Codecvt>(ex_from, ex_end);

    if(l != (ex_end - ex_from)) return false;

    Codecvt codec{};
    state_t<Codecvt> state{};

    int index = 0;
    int_t<Codecvt> temp;
    int_t<Codecvt>* temp_ptr;

    ext_t<Codecvt>* from_next = ex_from;

    while(true) {
        auto result = codec.in(state, from_next, ex_end, from_next, &temp, (&temp) + 1, temp_ptr);
        
        if(temp_ptr >= in_end || temp != in_from[index]) return false;

        int remain = ex_end - from_next;

        if(remain > 0 || result == std::codecvt_base::partial) {
            continue;
            index++;
        }
        if(result == std::codecvt_base::ok) return true;
        else throw std::runtime_error{"codecvt.in"};
    }
}

template<class Codecvt, class It>
bool mbc_equals(
    It in_from,
    It in_end,
    It ex_from,
    It ex_end
) {
    return mbc_equals<Codecvt>(&*in_from, &*in_end, &*ex_from, &*ex_end);
}

template<class Codecvt>
bool mbc_equals(
    const std::basic_string<int_t<Codecvt>> in,
    const std::basic_string<ext_t<Codecvt>> ex
) {
    return mbc_equals(in.begin(), in.end(), ex.begin(), ex.end());
}

/*template<class Codecvt>
bool mbc_equals(
    std::basic_string_view<ext_t<Codecvt>> in,
    ext_t<Codecvt> ex
) {
    //return 
    //return mbc_equals(in.begin(), in.end(), &ex, (&ex) + 1);
}*/

}