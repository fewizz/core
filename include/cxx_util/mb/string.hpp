#pragma once

#include <algorithm>
#include <compare>
#include <corecrt.h>
#include <cstddef>
#include <cstring>
#include <locale>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include "character_iterator.hpp"
#include "string_def.hpp"

namespace mb {

namespace internal {
    
    template<class Base, enc::encoding Encoding>
    struct common : protected Base {
        using traits_type      = typename Base::traits_type;
        using value_type       = character_view<Encoding>;
        using size_type        = typename Base::size_type;
        using difference_type  = typename Base::difference_type;
        using pointer          = typename Base::pointer;
        using const_pointer    = typename Base::const_pointer;
        using reference        = value_type&;
        using const_reference  = const value_type&;
        using iterator         = character_iterator<Encoding>;
        using const_iterator   = const character_iterator<Encoding>;
        static const
            size_type npos     = Base::npos;

        using char_type        = typename Encoding::char_type;
        using encoding_type    = Encoding;

        // constructor

        using Base::Base;
        
        common(const Base& str) : Base( str ) {}
        common(Base&& str) : Base( std::move(str) ) {}
        common(iterator b, iterator e) : Base( b.cur, e.cur ) {}
        common(const mb::character<Encoding>& ch) : Base( ch.begin(), ch.end() ) {}
        common(mb::character_view<Encoding> ch) : Base( ch.begin(), ch.end() ) {}

        using Base::operator = ;

        // element access
        
        value_type at(size_type index) const {
            auto prev_index = index;
            for(auto ch : *this)
                if(index-- == 0) return ch;

            throw std::out_of_range {
                "size: " + std::to_string(size()) +
                ", requested index: " + std::to_string(prev_index)
            };
        }

        value_type operator [] (size_type index) const {
            return at(index);
        }

        value_type front() const {
            return *begin();
        }

        using Base::data;

        operator std::basic_string_view<char_type> () const {
            return to_string_view<char_type>();
        }
        
        // iterators

        iterator begin() {
            return { data(), data(), data() + raw_size() };
        }

        const_iterator begin() const {
            return { data(), data(), data() + raw_size() };
        }

        const_iterator cbegin() const { return begin(); }

        iterator end() {
            return { data(), data() + raw_size() };
        }

        const_iterator end() const {
            return { data(), data() + raw_size() };
        }

        const_iterator cend() const { return end(); }

        auto raw_begin() const { return Base::begin(); }
        auto raw_end() const { return Base::end(); }

        // capacity

        using Base::empty;

        size_type size() const { return std::distance(begin(), end()); }

        auto length() const { return size(); }

        size_type raw_size() const { return Base::size(); }

        // operations
        
        using Base::compare;
        using Base::swap;

        // search
        
        size_type find(std::integral auto ch, size_type pos = 0) const {
            return find_first_of(ch, pos);
        }

        size_type find_first_of(std::integral auto ch, size_type pos = 0) const {
            auto sv = to_string_view();
            for(auto ch0 : sv.substr(pos)) {
                if(ch0 == ch) return pos;
                pos++;
            }
            return npos;
        }

        // other

        friend std::strong_ordering operator <=> (const common& a, const common& b) = default;

        template<enc::encoding Encoding0>
        mb::basic_string<Encoding0> convert() const {
            auto from = util::template from<Encoding>(data(), data() + Base::size());

            if(from.template to_always_noconv<Encoding0>()) {
                return { raw_begin(), raw_end() };
            }

            std::basic_string<typename Encoding0::char_type> str;
            str.resize(
                from.template to_length<Encoding0>()
            );

            from.template to<Encoding0>(
                str.data(), str.data() + str.size()
            );

            return { std::move(str) };
        }

        template<enc::encoding Encoding0>
        mb::basic_string<Encoding0> to_string() const {
            return convert<Encoding0>();
        }

        template<std::integral CharT = char_type>
        requires( sizeof(CharT) == sizeof(char_type) )
        std::basic_string_view<CharT> to_string_view() const {
            return {
                (CharT*) data(), (CharT*) data() + raw_size()
            };
        }

        template<std::integral CharT = char_type>
        auto to_string() const {
            return std::basic_string<CharT> { to_string_view<CharT>() };
        }
    };

    template<class Base, class Encoding>
    inline bool operator == (const common<Base, Encoding>& a, const typename Encoding::char_type* b) {
        return ((Base&)a).compare(b) == 0;
    }
}

template<
    enc::encoding Encoding,
    class Traits,
    class Allocator
>
struct basic_string : internal::common<std::basic_string<typename Encoding::char_type>, Encoding> {
    using char_type       = typename Encoding::char_type;
    using string_type     = std::basic_string<char_type, Traits, Allocator>;
    using base_type       = internal::common<string_type, Encoding>;
    using allocator_type  = typename string_type::allocator_type;
    using base_type::base_type;
    using typename  base_type::iterator;
    using typename base_type::const_iterator;
    using string_type::append;
    using string_type::get_allocator;

    // constructor
    basic_string(const mb::basic_string_view<Encoding> str) :
        base_type(str.data(), str.data() + str.raw_size()) {}

    basic_string& operator = (const mb::basic_string_view<Encoding> str) {
        (base_type&)*this = str.template to_string_view();
        return *this;
    }

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

    iterator insert(const_iterator pos, const char_type* ch) {
        typename string_type::size_type offset = pos.cur - string_type::data();
        string_type::insert(offset, ch);
        return {string_type::data(), string_type::data() + offset, string_type::data() + string_type::size()};
    }

    iterator insert(const_iterator pos, character_view<Encoding> ch) {
        return insert(pos, ch.template to_string<char_type>().data());
    } 

    iterator insert(const_iterator pos, const basic_string& ch) {
        return insert(pos, ch.data());
    }

    void push_back(character_view<Encoding> ch) {
        insert(base_type::end(), ch);
    }
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
template<enc::encoding E, class T, class A>
mb::basic_string<E, T, A> operator + (const mb::basic_string<E, T, A>& l, const typename E::char_type* r) {
    return internal::sum<E, T, A>(l.data(), l.raw_size(), r, T::length(r));
}

// char_type* + self
template<enc::encoding E, class T, class A>
mb::basic_string<E, T, A> operator + (const typename E::char_type* l, const mb::basic_string<E, T, A>& r) {
    return internal::sum<E, T, A>(l, T::length(l), r.data(), r.raw_size());
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

template<enc::encoding Encoding, class Traits>
struct basic_string_view :
internal::common<std::basic_string_view<typename Encoding::char_type, Traits>, Encoding> {
    using char_type = typename Encoding::char_type;
    using string_view_type = std::basic_string_view<char_type, Traits>;
    using base_type = mb::internal::common<string_view_type, Encoding>;
    using size_type = typename base_type::size_type;

    using base_type::base_type;

    basic_string_view substr(size_type pos = 0, size_type count = base_type::npos) const {
        auto b = base_type::begin() + pos;
        auto e = count == base_type::npos ? base_type::end() : b + count;
        return { b, e };
    }
};

// basic_string
using utf8_string = mb::basic_string<enc::utf8>;

class utf16_string : public mb::basic_string<enc::utf16> {
    using base = mb::basic_string<enc::utf16>;
public:
    using base::base;

    operator std::wstring_view() const {
        return {(wchar_t*)data(), (wchar_t*)data() + base_type::size()};
    }
};

using ascii_string = mb::basic_string<enc::ascii>;

// basic_string_view
using utf8_string_view = mb::basic_string_view<enc::utf8>;

class utf16_string_view : mb::basic_string_view<enc::utf16> {
    using base = mb::basic_string_view<enc::utf16>;
public:
    using base::base;

    operator std::wstring_view() const noexcept {
        return {(wchar_t*)data(), (wchar_t*)data() + base_type::size()};
    }
};

using ascii_string_view = mb::basic_string_view<enc::ascii>;
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