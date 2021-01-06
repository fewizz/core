#pragma once

#include "string_def.hpp"
#include "character.hpp"
#include "character_iterator.hpp"
#include <type_traits>

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
    common(iterator b, iterator e) :
    Base( mb::internal::current(b), mb::internal::current(e) - mb::internal::current(b) ) {}
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
    
    size_type find(auto ch, size_type pos = 0) const requires(std::is_integral_v<decltype(ch)>){
        return find_first_of(ch, pos);
    }

    size_type find_first_of(auto ch, size_type pos = 0) const requires(std::is_integral_v<decltype(ch)>){
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
        auto from = enc::template from<Encoding>(data(), data() + Base::size());

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

    template<class CharT = char_type>
    requires( sizeof(CharT) == sizeof(char_type) && std::is_integral_v<CharT>)
    std::basic_string_view<CharT> to_string_view() const {
        return {
            (CharT*) data(), raw_size()
        };
    }

    template<class CharT = char_type>
    requires(std::is_integral_v<CharT>)
    auto to_string() const {
        return std::basic_string<CharT> { to_string_view<CharT>() };
    }
};

template<class Base, class Encoding>
inline bool operator == (const common<Base, Encoding>& a, const typename Encoding::char_type* b) {
    return ((Base&)a).compare(b) == 0;
}
}

template<class CharT>
struct char_or_wchar {
    using type =
        std::conditional_t<
            sizeof(CharT) == sizeof(char),
            char,
            std::conditional_t<sizeof(CharT) == sizeof(wchar_t), wchar_t, void>
        >;
};

template<class CharT>
using char_or_wchar_t = typename char_or_wchar<CharT>::type;

}