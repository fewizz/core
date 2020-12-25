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
unsigned converted_mbc_length(
    const ext_t<Codecvt>* begin,
    const ext_t<Codecvt>* end
) {
    Codecvt codec{};
    state_t<Codecvt> state{};

    int_t<Codecvt> temp;
    int_t<Codecvt>* temp_ptr;

    unsigned result = 0;

    while(true) {
        const ext_t<Codecvt>* begin_next;
        begin += codec.length(state, begin, end, 1);
        result++;

        if(begin >= end) return result;
    }
}

template<class Codecvt>
std::basic_string<int_t<Codecvt>> convert_mbc(
    const ext_t<Codecvt>* begin,
    const ext_t<Codecvt>* end
) {
    Codecvt codec{};

    if(codec.always_noconv()) return {begin, end};

    std::basic_string<int_t<Codecvt>> result(
        converted_mbc_length<Codecvt>(begin, end),
        0
    );

    state_t<Codecvt> state{};

    const ext_t<Codecvt>* temp_ext;
    int_t<Codecvt>* temp_int;

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

template<class Codecvt, class It>
std::basic_string_view<ext_t<Codecvt>> next_mbc(It& begin, It end) {
    int l = first_mbc_length<Codecvt>(&*begin, &*end);
    begin += l;
    return {begin - l, begin};
}

template<class Codecvt, class It>
std::basic_string_view<ext_t<Codecvt>> next_converted_mbc(It& begin, It end) {
    int l = first_converted_length<Codecvt>(&*begin, &*end);
    begin += l;
    
    return {begin - l, begin};
}

/*template<class Codecvt>
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
}*/

/*template<class Codecvt>
bool mbc_equals(
    std::basic_string_view<ext_t<Codecvt>> in,
    ext_t<Codecvt> ex
) {
    //return 
    //return mbc_equals(in.begin(), in.end(), &ex, (&ex) + 1);
}*/

}