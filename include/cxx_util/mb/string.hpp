#pragma once

#include <algorithm>
#include <compare>
#include <corecrt.h>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include "common.hpp"

namespace mb {

namespace internal {
template<
    enc::encoding Encoding,
    class Traits = std::char_traits<typename Encoding::char_type>,
    class Allocator = std::allocator<typename Encoding::char_type>
>
struct basic_string : internal::common<std::basic_string<typename Encoding::char_type>, Encoding> {
    using char_type       = typename Encoding::char_type;
    using string_type     = std::basic_string<char_type, Traits, Allocator>;
    using base_type       = internal::common<string_type, Encoding>;
    using allocator_type  = typename string_type::allocator_type;
    using typename  base_type::iterator;
    using typename base_type::const_iterator;
    using typename base_type::size_type;
    using string_type::append;
    using string_type::get_allocator;

    using base_type::base_type;
    
    // constructor
    template<enc::encoding E>
    basic_string(const mb::basic_string<E> str)
    : base_type( str.template to_string<Encoding, char_type>() ) {}
    
    template<enc::encoding E>
    basic_string(const mb::basic_string_view<E> str)
    : base_type( str.template to_string<Encoding, char_type>() ) {}

    template<enc::encoding E>
    basic_string& operator = (const mb::basic_string_view<E> str) {
        (base_type&)*this = str.template to_string<Encoding, char_type>();
        return *this;
    }

    template<class CharT>
    basic_string(const std::basic_string_view<CharT> str)
    : basic_string( mb::basic_string_view<enc::encoding_from_char_t<CharT>>{ str } ) { }

    template<class CharT>
    basic_string(const std::basic_string<CharT>& str)
    : basic_string( mb::basic_string<enc::encoding_from_char_t<CharT>>{ str } ) { }

    template<class CharT>
    basic_string(const CharT* str)
    : basic_string( mb::basic_string_view<enc::encoding_from_char_t<CharT>>{ str } ) { }

    using string_type::clear;
    using string_type::max_size;

    int compare(mb::basic_string_view<Encoding> str) {
        return string_type::compare(0, string_type::size(), str.data(), str.raw_size());
    }

    mb::basic_string_view<Encoding> to_string_view() const {
        return {
            string_type::operator std::basic_string_view<char_type>()
        };
    }

    operator mb::basic_string_view<Encoding> () const {
        return to_string_view();
    }

    using string_type::operator +=;

    template<class CharT>
    basic_string& operator += (const CharT* str) {
        string_type::operator += (basic_string{str}.template to_string<char_type>() );
    }

    template<class CharT> requires(std::is_integral_v<CharT>)
    iterator insert(const_iterator pos, const CharT* ch) {
        auto converted = mb::basic_string_view<enc::encoding_from_char_t<CharT>>{ch}.template to_string<Encoding>();

        size_type offset = current(pos) - string_type::data();
        string_type::insert(offset, converted.data(), converted.raw_size());
        return {string_type::data(), string_type::data() + offset, string_type::data() + string_type::size()};
    }

    iterator insert(const_iterator pos, character_view<Encoding> ch) {
        return insert(pos, ch.template to_string<char_type>().data());
    }

    iterator insert(const_iterator pos, const basic_string& str) {
        return insert(pos, str.template to_string<Encoding>().data());
    }

    template<enc::encoding E>
    iterator insert(const_iterator pos, const basic_string_view<E> str) {
        return insert(pos, str.template to_string<Encoding>().data());
    }

    void push_back(character_view<Encoding> ch) {
        insert(base_type::end(), ch);
    }

    template<class CharT> requires(sizeof(CharT) <= sizeof(char_type))
    void push_back(CharT ch) {
        string_type::push_back((char_type)ch);
    }

    basic_string substr(size_type pos = 0, size_type count = base_type::npos) const {
        auto b = base_type::begin() + pos;
        auto e = count == base_type::npos ? base_type::end() : b + count;
        return { b, e };
    }
};

}

template<
    enc::encoding Encoding,
    class Traits,
    class Allocator
>
struct basic_string : public internal::basic_string<Encoding, Traits, Allocator> {
    using internal::basic_string<Encoding, Traits, Allocator>::basic_string;
};

namespace internal {
    template<enc::encoding E, class T, class A>
    mb::basic_string<E, T, A>
    sum(
        const typename E::char_type* l, size_t count_l,
        const typename E::char_type* r, size_t count_r
    ) {
        std::basic_string<typename E::char_type> result;
        result.reserve(count_l + count_r);

        result.append(l, count_l);
        result.append(r, count_r);

        return {result};
    }
}

// self
template<enc::encoding E, class T, class A>
mb::basic_string<E, T, A> operator + (const mb::basic_string<E, T, A>& l, const mb::basic_string<E, T, A>& r) {
    return internal::sum<E, T, A>(l.data(), l.raw_size(), r.data(), r.raw_size());
}

// self + char_type*
template<enc::encoding E, class T, class A, class CharT>
mb::basic_string<E, T, A> operator + (const mb::basic_string<E, T, A>& l, const CharT* r) {
    //return internal::sum<E, T, A>(l.data(), l.raw_size(), r, T::length(r));
    mb::basic_string<E, T, A> res{l};
    res.insert(res.end(), r);
    return res;
}

// char_type* + self
template<enc::encoding E, class T, class A, class CharT> 
mb::basic_string<E, T, A> operator + (const CharT* l, const mb::basic_string<E, T, A>& r) {
    //return internal::sum<E, T, A>(l, T::length(l), r.data(), r.raw_size());
    mb::basic_string<E, T, A> res{r};
    res.insert(res.begin(), l);
    return res;
}

// self + std::string
template<enc::encoding E, class T, class A>
mb::basic_string<E, T, A> operator + (
    const mb::basic_string<E, T, A>& l,
    std::basic_string<typename E::char_type, T, A> r
) {
    return internal::sum<E, T, A>(l.data(), l.raw_size(), r.data(), r.size());
}

// std::string + self
template<enc::encoding E, class T, class A>
mb::basic_string<E, T, A> operator + (
    std::basic_string<typename E::char_type, T, A> l,
    const mb::basic_string<E, T, A>& r
) {
    return internal::sum<E, T, A>(l.data(), l.size(), r.data(), r.raw_size());
}

using ascii_string = mb::basic_string<enc::ascii>;
using utf8_string = mb::basic_string<enc::utf8>;

template<>
class basic_string<enc::utf16> : public internal::basic_string<enc::utf16> {
    using base = mb::internal::basic_string<enc::utf16>;
public:
    using base::base;

    operator std::wstring_view() const {
        return {(wchar_t*)data(), raw_size()};
    }
};

using utf16_string = basic_string<enc::utf16>;

template<class T>
std::basic_ostream<char, T>&
    operator << (std::basic_ostream<char, T>& os,
               const utf8_string& str) {
    return os << str.template to_string<char>();
}

template<class T>
std::basic_ostream<char, T>&
    operator << (std::basic_ostream<char, T>& os,
               const ascii_string& str) {
    return os << str.template to_string<char>();
}

template<class T>
std::basic_ostream<wchar_t, T>&
    operator << (std::basic_ostream<char, T>& os,
               const utf16_string& str) {
    return os << str.template to_string<wchar_t>();
}

namespace internal {

// from libc++/istream
template<class CharT, class T = std::char_traits<CharT>, class MBString>
std::basic_istream<CharT, T>&
formatted_input (std::basic_istream<CharT, T>& is, MBString& str) {
    std::ios_base::iostate state = std::ios_base::goodbit;
    typename std::basic_istream<CharT, T>::sentry sen(is);

    if (sen) {
        try {
            str.clear();

            std::streamsize max = is.width();
            if (max <= 0)
                max = str.max_size();
            if (max <= 0)
                max = std::numeric_limits<std::streamsize>::max();
            
            std::streamsize size = 0;
            const std::ctype<CharT>& ctype = use_facet<std::ctype<CharT> >(is.getloc());

            while (size < max) {
                typename T::int_type char_code = is.rdbuf()->sgetc();
                if (T::eq_int_type(char_code, T::eof())) {
                   state |= std::ios_base::eofbit;
                   break;
                }
                CharT ch = T::to_char_type(char_code);

                if (ctype.is(ctype.space, ch))
                    break;
                
                str.push_back((typename MBString::char_type) ch);
                ++size;
                is.rdbuf()->sbumpc();
            }
            is.width(0);
            if (size == 0)
               state |= std::ios_base::failbit;
        }
        catch (...) {
            state |= std::ios_base::badbit;
            is.setstate(state);
            if (is.exceptions() & std::ios_base::badbit)
                throw;
        }
        is.setstate(state);
    }
    return is;
}

}

template<class T>
std::basic_istream<char, T>&
operator >> (std::basic_istream<char, T>& is, utf8_string& str) {
    return internal::formatted_input<char, T>(is, str);
}

template<class T>
std::basic_istream<char, T>&
operator >> (std::basic_istream<char, T>& is, ascii_string& str) {
    return internal::formatted_input<char, T>(is, str);
}

template<class T>
std::basic_istream<wchar_t, T>&
operator >> (std::basic_istream<wchar_t, T>& is, utf16_string& str) {
    return internal::formatted_input<wchar_t, T>(is, str);
}

//

template<class T>
concept string = requires {
    typename T::encoding_type;
} && std::is_same_v<T, mb::basic_string<typename T::encoding_type>>;

template<class T>
struct is_string : std::false_type {};
template<string T>
struct is_string<T> : std::true_type {};

}