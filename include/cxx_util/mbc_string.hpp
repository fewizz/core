#pragma once

#include <string_view>
#include <type_traits>
#include "cxx_util/mbc.hpp"

namespace util {

namespace mb {

namespace internal {

    template<class Parent, class CharT, class Codecvt = void>
    struct _character : Parent {
        using Parent::Parent;

        _character(const Parent& parent):Parent(parent){};
        _character(Parent&& parent):Parent(std::move(parent)){};

        _character<Parent, typename Codecvt::intern_type> convert()
        requires (not std::is_same_v<Codecvt, void>)
        {
            auto temp = Parent::begin();
            return util::next_mbc<Codecvt>(temp, Parent::end());
        }

        template<class Codecvt0>
        _character<Parent, CharT> convert()
        requires (std::is_same_v<Codecvt, void>)
        {
            auto temp = Parent::begin();
            return util::next_mbc<Codecvt0>(temp, Parent::end());
        }

        bool operator == (CharT ch) {
            if(this->size() != 1) return false;
            return this->at(0) == ch;
        }
    };
}

template<class CharT, class Codecvt = void>
struct character_view : internal::_character<std::basic_string_view<CharT>, CharT, Codecvt> {
    using internal::_character<std::basic_string_view<CharT>, CharT, Codecvt>::_character;
};

template<class CharT, class Codecvt = void>
struct character : internal::_character<std::basic_string<CharT>, CharT, Codecvt> {
    using internal::_character<std::basic_string<CharT>, CharT, Codecvt>::_character;
};

template<class Codecvt>
struct string_iterator {
    using char_type = typename Codecvt::extern_type;
    using value_type = character_view<char_type, Codecvt>;

    const char_type* begin;
    const char_type* end;

    string_iterator(
        const char_type* begin,
        const char_type* end
    ) :
    begin{begin}, end{end} {}

    string_iterator(string_iterator&&)=default;
    string_iterator(const string_iterator&)=default;

    value_type operator * () {
        auto temp = begin;
        return {next_mbc<Codecvt>(temp, end)};
    }

    auto& operator ++ () {
        begin += first_mbc_length<Codecvt>(begin, end);
        return *this;
    }

    bool operator != (const string_iterator<Codecvt>& other) {
        return begin != other.begin;
    }
};

namespace internal {

    template<class Parent, class Codecvt>
    struct _string : protected Parent {
        using char_type = typename Codecvt::extern_type;
        using value_type = character_view<char_type, Codecvt>;
        using size_type = unsigned;
        using Parent::Parent;

        _string(const Parent& other):Parent(other) {}

        _string(string_iterator<Codecvt> b, string_iterator<Codecvt> e):Parent(b.begin, e.end) {}

        string_iterator<Codecvt> begin() {
            return {&*(Parent::begin()), &*(Parent::end())};
        }

        string_iterator<Codecvt> end() {
            return {&*(Parent::end()), &*(Parent::end())};
        }

        string_iterator<Codecvt> operator + (int offset) {
            string_iterator<Codecvt> b = *this;
            for(; offset > 0; ++b, offset--);
            return b;
        }

        size_type size() {
            size_type s = 0;
            for(auto b = begin(); b != end(); ++b, s++);
            return s;
        }

        std::basic_string_view<char_type> to_string_view() {
            return {Parent::begin(), Parent::end()};
        }

        std::basic_string<char_type> to_string() {
            return {Parent::begin(), Parent::end()};
        }

    };
}

template<class Codecvt>
struct string : internal::_string<std::basic_string<typename Codecvt::extern_type>, Codecvt> {
    using internal::_string<std::basic_string<typename Codecvt::extern_type>, Codecvt>::_string;
};

template<class Codecvt>
struct string_view : internal::_string<std::basic_string_view<typename Codecvt::extern_type>, Codecvt> {
    using internal::_string<std::basic_string_view<typename Codecvt::extern_type>, Codecvt>::_string;
};

}

}