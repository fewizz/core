#pragma once

#include <cstddef>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include "mbc.hpp"
#include "encoding.hpp"

namespace util {

namespace mb {

namespace internal {
    template<class Parent, class Encoding>
    struct _character;
}

template<class Encoding>
struct character;

namespace internal {

    template<class Parent, class Encoding>
    struct _character : Parent {
        using Parent::Parent;
        using char_type = typename Encoding::char_type;

        _character(const Parent& parent):Parent(parent){};
        _character(Parent&& parent):Parent(std::move(parent)){};

        _character(char_type ch):Parent(1, ch) {};

        auto data() const {
            return Parent::data();
        }

        auto size() const {
            return Parent::size();
        }

        bool operator != (std::integral auto ch) {
            return this->size() != 1 || Parent::front() != ch;
        }

        template<class T>
        bool operator == (const T& other) {
            return Parent::operator == (other);
        }

        template<class Encoding0>
        character<Encoding0> convert() {
            return {
                convert_mbc<typename util::codec<Encoding0, Encoding>::type>(
                    Parent::data(),
                    Parent::data() + Parent::size()
                )
            };
        }
    };
}

template<class Encoding>
struct character_view : internal::_character<std::basic_string_view<typename Encoding::char_type>, Encoding> {
    using internal::_character<std::basic_string_view<typename Encoding::char_type>, Encoding>::_character;
};

template<class Encoding>
struct character : internal::_character<std::basic_string<typename Encoding::char_type>, Encoding> {
    using internal::_character<std::basic_string<typename Encoding::char_type>, Encoding>::_character;
};

template<class Encoding>
struct string_iterator {
    using char_type = typename Encoding::char_type;
    using value_type = character_view<Encoding>;
    using difference_type = std::ptrdiff_t;

    const char_type* begin;
    const char_type* end;

    string_iterator() {}
    string_iterator(string_iterator&& other) = default;
    string_iterator& operator=(string_iterator&& other) = default;
    string_iterator(const string_iterator& other) = default;
    string_iterator& operator=(const string_iterator& other) = default;

    string_iterator(
        const char_type* begin,
        const char_type* end
    ) :
    begin{begin}, end{end} {}

    value_type operator * () const {
        auto next_len = Encoding::first_char_length(begin, end);
        return {begin, begin + next_len};
    }

    auto& operator ++ () {
        begin += Encoding::first_char_length(begin, end);
        return *this;
    }

    auto operator ++ (int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    bool operator != (const string_iterator& other) const {
        return begin != other.begin;
    }

    bool operator == (const string_iterator& other) const {
        return begin == other.begin;
    }
};

namespace internal {
    template<class Parent, class Encoding>
    struct _string;
}

template<class Encoding>
struct string;

namespace internal {
    template<class Parent, class Encoding>
    struct _string : protected Parent {
        using char_type = typename Encoding::char_type;

        using value_type = character_view<Encoding>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = string_iterator<Encoding>;
        using const_iterator = const string_iterator<Encoding>;
        using difference_type = typename Parent::difference_type;
        using size_type = typename Parent::size_type;

        using Parent::Parent;

        _string(const auto& other) : Parent(other) {}
        _string(auto&& other) : Parent(std::move(other)) {}

        _string& operator = (const auto& other) {
            Parent::operator = (other);
            return *this;
        }

        _string& operator = (auto&& other) {
            Parent::operator = (std::move(other));
            return *this;
        }

        _string(iterator b, iterator e):Parent(b.begin, e.end) {}

        iterator begin() {
            return {&*(Parent::begin()), &*(Parent::end())};
        }

        const_iterator cbegin() {
            return begin();
        }

        iterator end() {
            return {&*(Parent::end()), &*(Parent::end())};
        }

        const_iterator cend() {
            return end();
        }

        bool operator == (const auto& other) {
            return Parent::operator == (other);
        }

        bool operator != (const auto& other) {
            return Parent::operator != (other);
        }

        void swap(_string& other) {
            Parent::swap(other);
        }

        /*iterator operator + (int offset) {
            iterator b = *this;
            for(; offset > 0; ++b, offset--);
            return b;
        }*/

        
        value_type operator [] (unsigned offset) {
            auto it = begin();
            while(offset > 0) {
                if(it == end()) throw std::runtime_error{"out of bounds"};
                ++it;
                if(it == end()) throw std::runtime_error{"out of bounds"};
                --offset;
            }
            return *it;
        }

        size_type size() {
            size_type s = 0;
            for(auto b = begin(); b != end(); ++b, s++);
            return s;
        }

        auto length() { return size(); }
        
        bool empty() {
            return Parent::empty();
        }

        auto data() const {
            return Parent::data();
        }

        auto& append(auto ch) {
            Parent::append(ch);
            return *this;
        }

        template<class Encoding0>
        string<Encoding0> convert() {
            return {
                convert_mbc<typename util::codec<Encoding0, Encoding>::type>(
                    Parent::data(),
                    Parent::data() + Parent::size()
                )
            };
        }

        //operator std::

        std::basic_string_view<char_type> to_string_view() const {
            return {Parent::begin(), Parent::end()};
        }

        std::basic_string<char_type> to_string() const {
            return {Parent::begin(), Parent::end()};
        }

    };
}

template<class Encoding>
struct string : internal::_string<std::basic_string<typename Encoding::char_type>, Encoding> {
    using internal::template _string<std::basic_string<typename Encoding::char_type>, Encoding>::_string;
};

template<class Encoding>
struct string_view : internal::_string<std::basic_string_view<typename Encoding::char_type>, Encoding> {
    using internal::template _string<std::basic_string_view<typename Encoding::char_type>, Encoding>::_string;
};

}

}