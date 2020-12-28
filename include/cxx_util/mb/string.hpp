#pragma once

#include <compare>
#include <cstddef>
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

template<enc::encoding Encoding>
struct basic_string_view;

namespace internal {
    template<class Parent, enc::encoding Encoding>
    struct string : protected Parent {
        using traits_type      = typename Parent::traits_type;
        using value_type       = character_view<Encoding>;
        using size_type        = typename Parent::size_type;
        using difference_type  = typename Parent::difference_type;
        using pointer          = typename Parent::pointer;
        using const_pointer    = typename Parent::const_pointer;
        using reference        = value_type&;
        using const_reference  = const value_type&;
        using iterator         = character_iterator<Encoding>;
        using const_iterator   = const character_iterator<Encoding>;
        static const size_type npos = Parent::npos;

        using char_type        = typename Encoding::char_type;
        using encoding_type    = Encoding;

        using Parent::Parent;

        string(const auto& other) : Parent(other) {}
        string(const basic_string<Encoding>& other) : Parent(other.to_string_view()) {}
        string(const basic_string_view<Encoding>& other) : Parent(other.to_string_view()) {}

        string& operator = (const auto& other) {
            Parent::operator = (other);
            return *this;
        }

        string& operator = (auto&& other) {
            Parent::operator = (std::move(other));
            return *this;
        }

        string(iterator b, iterator e) : Parent(b.begin, e.begin) {}

        iterator begin() {
            return {Parent::data(), Parent::data()+Parent::size()};
        }

        const_iterator begin() const {
            return {Parent::data(), Parent::data()+Parent::size()};
        }

        const_iterator cbegin() const { return begin(); }

        iterator end() {
            return {Parent::data()+Parent::size(), Parent::data()+Parent::size()};
        }

        const_iterator end() const {
            return {Parent::data()+Parent::size(), Parent::data()+Parent::size()};
        }

        const_iterator cend() const { return end(); }

        int operator <=> (const auto& other) const {
            return Parent::compare(other);
        }

        void swap(string& other) { Parent::swap(other); }
        
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
        
        using Parent::empty;
        using Parent::data;

        auto& append(auto ch) {
            Parent::append(ch);
            return *this;
        }

        template<enc::encoding Encoding0>
        mb::basic_string<Encoding0> convert() const {
            auto from = util::template from<Encoding>(data(), data() + Parent::size());

            if(from.template to_always_noconv<Encoding0>()) {
                return {data(), data() + Parent::size()};
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

        std::basic_string_view<char_type> to_string_view() const {
            return { Parent::begin(), Parent::end() };
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
            return operator <=> (((Parent&) *this), other);
        }

        auto operator <=> (const auto& other) const {
            return to_string_view() <=> other;
        }

        auto operator <=> (const mb::basic_string_view<Encoding>& other) const {
            return to_string_view() <=> other.to_string_view();
        }

        bool operator <=> (const mb::basic_string<Encoding>& other) const {
            return to_string_view() <=> other.to_string_view();
        }

        bool operator == (const auto& other) const {
            return to_string_view() == other;
        }

        bool operator == (const mb::basic_string_view<Encoding>& other) const {
            return to_string_view() == other.to_string_view();
        }

        bool operator == (const mb::basic_string<Encoding>& other) const {
            return to_string_view() == other.to_string_view();
        }

        mb::basic_string_view<Encoding> substr(size_type pos = 0, size_type count = npos) const {
            auto b = begin() + pos;
            auto e = count == npos ? end() : b + count;
            return {b, e};
        }

        value_type front() const {
            return *begin();
        }
    };
}

template<
    enc::encoding Encoding,
    class Traits,
    class Allocator
>
struct basic_string : internal::string<std::basic_string<typename Encoding::char_type>, Encoding> {
    using string_type = std::basic_string<typename Encoding::char_type, Traits, Allocator>;

    using base_type = internal::string<string_type, Encoding>;
        
    using allocator_type = typename string_type::allocator_type;

    using base_type::base_type;

    operator basic_string_view<Encoding> () const {
        return {
            string_type::operator std::basic_string_view<typename base_type::char_type>()
        };
    }

    template<std::integral CharT = typename base_type::char_type>
    std::basic_string<CharT> to_string() const &
    requires( sizeof(CharT) == sizeof(typename base_type::char_type) ) {
        return { (CharT*) base_type::data(), (CharT*) base_type::data() + string_type::size() };
    }

    string_type to_string() const && {
        return std::move(*this);
    }

    template<enc::encoding Encoding0>
    auto to_string() const {
        return base_type::template convert <Encoding0>().to_string();
    }
};

template<enc::encoding Encoding>
struct basic_string_view : mb::internal::string<std::basic_string_view<typename Encoding::char_type>, Encoding> {
    using string_view_type = std::basic_string_view<typename Encoding::char_type>;

    using base_type = mb::internal::string<string_view_type, Encoding>;

    using base_type::base_type;
};

template<enc::encoding Encoding>
auto operator <=> (const mb::basic_string<Encoding>& l, const auto& r) {
    return l.operator <=> (r);
}

template<enc::encoding Encoding>
auto operator <=> (const mb::basic_string_view<Encoding>& l, const auto& r) {
    return l.operator <=> (r);
}

template<enc::encoding Encoding>
bool operator == (const mb::basic_string<Encoding>& l, const auto& r) {
    return l.operator == (r);
}

template<enc::encoding Encoding>
bool operator == (const mb::basic_string_view<Encoding>& l, const auto& r) {
    return l.operator == (r);
}

using utf8_string = mb::basic_string<enc::utf8>;
using utf16_string = mb::basic_string<enc::utf16>;
using ascii_string = mb::basic_string<enc::ascii>;

using utf8_string_view = mb::basic_string_view<enc::utf8>;
class utf16_string_view : mb::basic_string_view<enc::utf16> {
    using base = mb::basic_string_view<enc::utf16>;
public:
    using base::base;

    operator std::wstring_view() const {
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