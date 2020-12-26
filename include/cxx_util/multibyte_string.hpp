#pragma once

#include <compare>
#include <cstddef>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include "codecvt.hpp"
#include "encoding.hpp"

namespace util {

namespace mb {

namespace internal {
    template<class Parent, class Encoding>
    struct _character;
}

template<class Encoding>
struct character;

template<class Encoding>
struct character_view;

namespace internal {

    template<class Parent, class Encoding>
    struct _character : Parent {
        using Parent::Parent;
        using char_type = typename Encoding::char_type;

        _character(const Parent& parent) : Parent(parent){};
        _character(Parent&& parent) : Parent(std::move(parent)){};

        _character(char_type ch) : Parent(1, ch) {};

        auto data() const { return Parent::data();  }

        auto size() const { return Parent::size(); }

        bool operator == (std::integral auto ch) const {
            return this->size() == 1 && Parent::front() == ch;
        }

        bool operator != (std::integral auto ch) const {
            return !(*this == ch);
        }

        template<class Encoding0>
        character<Encoding0> convert() const {
            return {
                util::codecvt<typename util::enc::codec<Encoding0, Encoding>::type>::convert_in(
                    Parent::data(),
                    Parent::data() + Parent::size()
                )
            };
        }

        operator character_view<Encoding> ()
        const requires(std::is_same_v<Parent, std::basic_string<char_type>>) {
            return { Parent::operator std::template basic_string_view<char_type>() };
        }

        std::basic_string<char_type> to_string() const {
            return { Parent::begin(), Parent::end() };
        }

        template<class Encoding0>
        auto to_string() const {
            return convert <Encoding0>().to_string();
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
    string_iterator& operator = (string_iterator&& other) = default;
    string_iterator(const string_iterator& other) = default;
    string_iterator& operator = (const string_iterator& other) = default;

    string_iterator(
        const char_type* begin,
        const char_type* end
    ) :
    begin{begin}, end{end} {
        if(begin > end) throw std::out_of_range{"passed end"};
    }

    value_type operator * () const {
        if(begin >= end) throw std::out_of_range{"passed end"};
        auto next_len = Encoding::first_char_length(begin, end);
        return {begin, begin + next_len};
    }

    auto& operator ++ () {
        if(begin >= end) throw std::out_of_range{"passed end"};
        begin += Encoding::first_char_length(begin, end);
        return *this;
    }

    auto operator ++ (int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    auto operator + (std::integral auto offset) {
        auto offset0 = offset;
        auto it = begin;

        if(it < end) {
            while(offset-- > 0) {
                ++it;
                if(it == end) break;
            }
            return string_iterator<Encoding>{it, end};
        }

        throw std::out_of_range{
            "requested index: " + std::to_string(offset0)
        };
    }

    auto operator <=> (const string_iterator& other) const {
        return begin <=> other.begin;
    }

    bool operator == (const string_iterator& other) const = default;
    bool operator != (const string_iterator& other) const = default;
};

namespace internal {
    template<class Parent, class Encoding>
    struct _string;
}

template<class Encoding>
struct basic_string;

template<class Encoding>
struct basic_string_view;

namespace internal {
    template<class Parent, class Encoding>
    struct _string : protected Parent {
        using char_type = typename Encoding::char_type;
        using encoding_type = Encoding;

        using value_type = character_view<Encoding>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = string_iterator<Encoding>;
        using const_iterator = const string_iterator<Encoding>;
        using difference_type = typename Parent::difference_type;
        using size_type = typename Parent::size_type;

        static constexpr size_type npos = Parent::npos;

        using Parent::Parent;

        _string(const auto& other) : Parent(other) {}
        _string(const basic_string<Encoding>& other) : Parent(other.to_string_view()) {}
        _string(const basic_string_view<Encoding>& other) : Parent(other.to_string_view()) {}

        _string& operator = (const auto& other) {
            Parent::operator = (other);
            return *this;
        }

        _string& operator = (auto&& other) {
            Parent::operator = (std::move(other));
            return *this;
        }

        _string(iterator b, iterator e) : Parent(b.begin, e.begin) {}

        iterator begin() const {
            return {&*(Parent::begin()), &*(Parent::end())};
        }

        const_iterator cbegin() const { return cbegin(); }

        iterator end() const {
            return {&*(Parent::end()), &*(Parent::end())};
        }

        const_iterator cend() const { return cend(); }

        int operator <=> (const auto& other) const {
            return Parent::compare(other);
        }

        void swap(_string& other) { Parent::swap(other); }
        
        value_type operator [] (unsigned index) const {
            auto it = begin();
            if(it != end()) {
                while(index-- > 0) {
                    ++it;
                    if(it == end()) break;
                }
                return *it;
            }
            throw std::out_of_range{
                "size: " + std::to_string(size()) +
                ", requested index: " + std::to_string(index)
            };
        }

        size_type size() const {
            size_type s = 0;
            for(auto b = begin(); b != end(); ++b, s++);
            return s;
        }

        auto length() const { return size(); }
        
        bool empty() const { return Parent::empty(); }

        auto data() const { return Parent::data(); }

        auto& append(auto ch) {
            Parent::append(ch);
            return *this;
        }

        template<class Encoding0>
        basic_string<Encoding0> convert() const {
            return {
                codecvt<typename util::enc::codec<Encoding0, Encoding>::type>::convert_in(
                    Parent::data(),
                    Parent::data() + Parent::size()
                )
            };
        }

        operator basic_string_view<Encoding> () const requires(std::is_same_v<Parent, std::basic_string<char_type>>) {
            return { Parent::operator std::basic_string_view<char_type>() };
        }

        std::basic_string_view<char_type> to_string_view() const {
            return { Parent::begin(), Parent::end() };
        }

        std::basic_string<char_type> to_string() const {
            return { Parent::begin(), Parent::end() };
        }

        template<class Encoding0>
        auto to_string() const {
            return convert <Encoding0>().to_string();
        }

        size_type find_first_of(std::integral auto ch, size_type pos = 0) const {
            for(auto ch0 : basic_string_view<Encoding>{begin() + pos, end()}) {
                if(ch0 == ch) return pos;
                pos++;
            }
            return npos;
        }

        size_type find(std::integral auto ch, size_type pos = 0) const {
            return find_first_of(ch, pos);
        }

        std::strong_ordering operator <=> (const auto& other) const {
            return operator <=> (((Parent&) *this), other);
        }

        bool operator < (const auto& other) const {
            return to_string_view() < other;
        }

        bool operator < (const basic_string_view<Encoding>& other) const {
            return to_string_view() < other.to_string_view();
        }

        bool operator < (const basic_string<Encoding>& other) const {
            return to_string_view() < other.to_string_view();
        }

        basic_string_view<Encoding> substr(size_type pos = 0, size_type count = npos) const {
            auto b = begin() + pos;
            auto e = count == npos ? end() : b + count;
            return {b, e};
        }

        value_type front() const {
            return *begin();
        }
    };
}

template<class Encoding>
struct basic_string : internal::_string<std::basic_string<typename Encoding::char_type>, Encoding> {
    using internal::template _string<std::basic_string<typename Encoding::char_type>, Encoding>::_string;
};

template<class Encoding>
bool operator < (const basic_string<Encoding>& l, const auto& r) {
    return l.operator < (r);
}

template<class Encoding>
bool operator < (const basic_string_view<Encoding>& l, const auto& r) {
    return l.operator < (r);
}

template<class Encoding>
struct basic_string_view : internal::_string<std::basic_string_view<typename Encoding::char_type>, Encoding> {
    using internal::template _string<std::basic_string_view<typename Encoding::char_type>, Encoding>::_string;
};

using utf8_string = basic_string<util::enc::utf8>;
using utf16_string = basic_string<util::enc::utf16>;
using ascii_string = basic_string<util::enc::ascii>;

using utf8_string_view = basic_string_view<util::enc::utf8>;
using ascii_string_view = basic_string_view<util::enc::ascii>;

template<class T>
concept string = requires {
    typename T::encoding_type;
} && std::is_same_v<T, util::mb::basic_string<typename T::encoding_type>>;

template<class T>
struct is_string : std::false_type {};
template<string T>
struct is_string<T> : std::true_type {};

}
}