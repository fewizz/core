#pragma once

#include <string_view>
#include "cxx_util/mbc.hpp"

namespace util {

template<class CharT, class Codecvt>
struct mbc_string_iterator {
    const CharT* begin;
    const CharT* end;

    mbc_string_iterator(
        const CharT* begin,
        const CharT* end
    ) :
    begin{begin}, end{end} {}

    mbc_string_iterator(mbc_string_iterator&&)=default;
    mbc_string_iterator(const mbc_string_iterator&)=default;

    using value_type = std::basic_string<CharT>;

    value_type operator *() {
        auto temp = begin;
        return next_mbc<Codecvt>(temp, end);
    }

    auto& operator ++() {
        begin += first_mbc_length<Codecvt>(begin, end);
        return *this;
    }

    bool operator != (const mbc_string_iterator<CharT, Codecvt>& other) {
        return begin != other.begin;
    }
};

template<class CharT, class Codecvt>
struct basic_mbc_string_view {
using size_type = unsigned;

std::basic_string_view<CharT> m_str;

mbc_string_iterator<CharT, Codecvt> begin() {
    return {&*m_str.begin(), &*m_str.end()};
}

mbc_string_iterator<CharT, Codecvt> end() {
    return {&*m_str.end(), &*m_str.end()};
}

size_type size() {
    size_type s = 0;
    for(auto b = begin(); b != end(); ++b, s++);
    return s;
}

};

template<class Codecvt>
using mbc_string_view = basic_mbc_string_view<char, Codecvt>;

template<class Codecvt>
using mbc_wstring_view = basic_mbc_string_view<wchar_t, Codecvt>;

}