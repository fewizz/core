#pragma once

#include "character.hpp"
#include <compare>
#include "string_def.hpp"

namespace mb {

template<enc::encoding Encoding>
class character_iterator {
    using char_type = typename Encoding::char_type;
    const char_type* begin;
    const char_type* cur;
    const char_type* end;

    template<class Base, enc::encoding Encoding0>
    friend class mb::internal::common;

    template<enc::encoding Encoding0, class Traits, class Allocator>
    friend struct mb::basic_string;

    template<enc::encoding Encoding0, class Traits>
    friend struct mb::basic_string_view;

public:

    using value_type = character_view<Encoding>;
    using difference_type = std::ptrdiff_t;

    character_iterator() {}
    character_iterator(character_iterator&& other) = default;
    character_iterator& operator = (character_iterator&& other) = default;
    character_iterator(const character_iterator& other) = default;
    character_iterator& operator = (const character_iterator& other) = default;

private:
    void check() const {
        if(begin > cur)
            throw std::out_of_range{ "begin passed current" };
        if(cur > end)
            throw std::out_of_range{ "current passed end" };
    }

    auto width() const {
        auto info = Encoding::first_char_width(cur, end);
        if(info.result != enc::request_result::ok) {
            throw std::runtime_error{ "getting multibyte character width" };
        }
        return info.size;
    }
public:
    character_iterator(
        const char_type* begin,
        const char_type* cur,
        const char_type* end
    ) :
    begin{ begin }, cur{ cur }, end{ end } {
        check();
    }

    character_iterator(
        const char_type* begin, const char_type* end
    ) :
    begin{ begin }, cur{ end }, end{ end } {}

    value_type operator * () const {
        check();
        
        return { cur, cur + width() };
    }

    auto& operator ++ () {
        check();
        cur += width();
        return *this;
    }

    auto operator ++ (int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    auto operator + (std::integral auto offset) const {
        auto offset0 = offset;
        auto it = cur;

        if(it < end) {
            while(offset-- > 0) {
                it += width();
                if(it >= end) break;
            }

            return character_iterator<Encoding>{ begin, it, end };
        }

        throw std::out_of_range{
            "requested offset: " + std::to_string(offset0)
        };
    }

    std::partial_ordering operator <=> (const character_iterator& other) const {
        if(begin != other.begin || end != other.end) return std::partial_ordering::unordered;
        return cur <=> other.cur;
    }

    bool operator == (const character_iterator& other) const = default;
    bool operator != (const character_iterator& other) const = default;
};

}