#pragma once

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

        bool operator != (std::integral auto ch) const {
            return this->size() != 1 || Parent::front() != ch;
        }

        template<class T>
        bool operator == (const T& other) const {
            return Parent::operator == (other);
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
    string_iterator& operator=(string_iterator&& other) = default;
    string_iterator(const string_iterator& other) = default;
    string_iterator& operator=(const string_iterator& other) = default;

    string_iterator(
        const char_type* begin,
        const char_type* end
    ) :
    begin{begin}, end{end} {}

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

        using Parent::Parent;

        _string(const auto& other) : Parent(other) {}
        _string(const basic_string<Encoding>& other) : Parent(other.to_string_view()) {}

        _string& operator = (const auto& other) {
            Parent::operator = (other);
            return *this;
        }

        _string& operator = (auto&& other) {
            Parent::operator = (std::move(other));
            return *this;
        }

        _string(iterator b, iterator e) : Parent(b.begin, e.end) {}

        iterator begin() const {
            return {&*(Parent::begin()), &*(Parent::end())};
        }

        const_iterator cbegin() const {
            return begin();
        }

        iterator end() const {
            return {&*(Parent::end()), &*(Parent::end())};
        }

        const_iterator cend() const {
            return end();
        }

        bool operator == (const auto& other) const {
            return Parent::operator == (other);
        }

        bool operator != (const auto& other) const {
            return Parent::operator != (other);
        }

        void swap(_string& other) {
            Parent::swap(other);
        }
        
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
        
        bool empty() const {
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
        basic_string<Encoding0> convert() const {
            return {
                codecvt<typename util::enc::codec<Encoding0, Encoding>::type>::convert_in(
                    Parent::data(),
                    Parent::data() + Parent::size()
                )
            };
        }

        operator basic_string_view<Encoding> () requires(std::is_same_v<Parent, std::basic_string<char_type>>) {
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

    };
}

template<class Encoding>
struct basic_string : internal::_string<std::basic_string<typename Encoding::char_type>, Encoding> {
    using internal::template _string<std::basic_string<typename Encoding::char_type>, Encoding>::_string;
};

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