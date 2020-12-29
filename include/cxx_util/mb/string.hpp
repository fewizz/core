#pragma once

#include <compare>
#include <cstddef>
#include <cstring>
#include <locale>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include "character_iterator.hpp"

namespace mb {

namespace internal {
    template<class Parent, enc::encoding Encoding>
    struct _string;
}

template<
    enc::encoding Encoding,
    class Traits = std::char_traits<typename Encoding::char_type>,
    class Allocator = std::allocator<typename Encoding::char_type>
>
struct basic_string;

template<enc::encoding Encoding, class Traits = std::char_traits<typename Encoding::char_type>>
struct basic_string_view;

namespace internal {
    template<class Base, enc::encoding Encoding>
    struct base : protected Base {
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

        using Base::Base;
        
        base(const Base& str) : Base(str) {}
        base(Base&& str) : Base(std::move(str)) {}

        base& operator = (const auto& other) {
            Base::operator = (other);
            return *this;
        }

        base& operator = (auto&& other) {
            Base::operator = (std::move(other));
            return *this;
        }

        base(iterator b, iterator e) : Base(b.begin, e.begin) {}

        iterator begin() {
            return {Base::data(), Base::data()+Base::size()};
        }

        const_iterator begin() const {
            return {data(), data()+Base::size()};
        }

        const_iterator cbegin() const { return begin(); }

        iterator end() {
            return {data()+Base::size(), data()+Base::size()};
        }

        const_iterator end() const {
            return {data()+Base::size(), data()+Base::size()};
        }

        const_iterator cend() const { return end(); }

        int operator <=> (const auto& other) const {
            return Base::compare(other);
        }

        void swap(base& other) { Base::swap(other); }
        
        value_type operator [] (unsigned index) const {
            auto prev_index = index;
            for(auto ch : *this)
                if(index-- == 0) return ch;

            throw std::out_of_range{
                "size: " + std::to_string(size()) +
                ", requested index: " + std::to_string(prev_index)
            };
        }

        size_type size() const {
            size_type s = 0;

            for(auto ch : *this)
                s++;
            
            return s;
        }

        auto length() const { return size(); }
        
        using Base::empty;
        using Base::data;

        size_type raw_size() const {
            return Base::size();
        }

        auto& append(auto ch) {
            Base::append(ch);
            return *this;
        }

        template<enc::encoding Encoding0>
        mb::basic_string<Encoding0> convert() const {
            auto from = util::template from<Encoding>(data(), data() + Base::size());

            if(from.template to_always_noconv<Encoding0>()) {
                return {data(), data() + Base::size()};
            }

            std::basic_string<typename Encoding0::char_type> str;
            str.resize(
                from.template to_length<Encoding0>()
            );

            from.template to<Encoding0>(
                str.data(), str.data() + str.size()
            );

            return {std::move(str)};
        }

        size_type find_first_of(std::integral auto ch, size_type pos = 0) const {
            for(auto ch0 : substr(pos)) {
                if(ch0 == ch) return pos;
                pos++;
            }
            return npos;
        }

        size_type find(std::integral auto ch, size_type pos = 0) const {
            return find_first_of(ch, pos);
        }

        std::strong_ordering operator <=> (const auto& other) const {
            return operator <=> (((Base&) *this), other);
        }

    protected:
        int sized_compare(size_type size, const char_type* str) const noexcept {
            auto this_size = raw_size();
            int result = traits_type::compare(data(), str, std::min(this_size, size));
            if (result != 0)
                return result;
            if (this_size < size)
                return -1;
            if (this_size > size)
                return 1;
            return 0;
        }
    public:

        int compare(const base& s) const noexcept { return sized_compare(s.raw_size(), s.data()); }
        int compare(const Base& s) const noexcept { return sized_compare(s.size(), s.data()); }
        int compare(const char_type* s) const noexcept { return sized_compare(traits_type::length(s), s); }

        mb::basic_string_view<Encoding, traits_type> substr(size_type pos = 0, size_type count = npos) const {
            auto b = begin() + pos;
            auto e = count == npos ? end() : b + count;
            return {b, e};
        }

        value_type front() const {
            return *begin();
        }

        operator mb::basic_string_view<Encoding> () const noexcept {
        return {
            Base::operator std::basic_string_view<char_type>()
        };
    }
    };
}

template<
    enc::encoding Encoding,
    class Traits,
    class Allocator
>
struct basic_string : internal::base<std::basic_string<typename Encoding::char_type>, Encoding> {
    using char_type = typename Encoding::char_type;
    using string_type = std::basic_string<char_type, Traits, Allocator>;

    using base_type = internal::base<string_type, Encoding>;
        
    using allocator_type = typename string_type::allocator_type;

    using base_type::base_type;

    template<std::integral CharT = char_type>
    std::basic_string<CharT> to_string() const &
    requires( sizeof(CharT) == sizeof(typename base_type::char_type) )
    {
        return { (CharT*) base_type::data(), (CharT*) base_type::data() + string_type::size() };
    }

    using string_type::operator std::basic_string_view<char_type>;
};

template<enc::encoding Encoding, class Traits>
struct basic_string_view : mb::internal::base<std::basic_string_view<typename Encoding::char_type, Traits>, Encoding> {
    using string_view_type = std::basic_string_view<typename Encoding::char_type, Traits>;

    using base_type = mb::internal::base<string_view_type, Encoding>;

    using base_type::base_type;
};

template<enc::encoding Encoding>
bool operator == (const mb::basic_string<Encoding>& l, const auto& r) {
    return l.compare(r) == 0;
}

template<enc::encoding Encoding>
auto operator <=> (const mb::basic_string<Encoding>& l, const auto& r) {
    return l.compare(r);
}

using utf8_string = mb::basic_string<enc::utf8>;
class utf16_string : public mb::basic_string<enc::utf16> {
    using base = mb::basic_string<enc::utf16>;
public:
    using base::base;

    operator std::wstring_view() const noexcept {
        return {(wchar_t*)data(), (wchar_t*)data() + base_type::size()};
    }
};
using ascii_string = mb::basic_string<enc::ascii>;

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

template<class T>
concept string = requires {
    typename T::encoding_type;
} && std::is_same_v<T, mb::basic_string<typename T::encoding_type>>;

template<class T>
struct is_string : std::false_type {};
template<string T>
struct is_string<T> : std::true_type {};

}