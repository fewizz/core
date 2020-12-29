#pragma once

#include "character.hpp"

namespace mb {

template<enc::encoding Encoding>
struct character_iterator {
    using char_type = typename Encoding::char_type;
    using value_type = character_view<Encoding>;
    using difference_type = std::ptrdiff_t;

    const char_type* begin;
    const char_type* end;

    character_iterator() {}
    character_iterator(character_iterator&& other) = default;
    character_iterator& operator = (character_iterator&& other) = default;
    character_iterator(const character_iterator& other) = default;
    character_iterator& operator = (const character_iterator& other) = default;

    character_iterator(
        const char_type* begin,
        const char_type* end
    ) :
    begin{begin}, end{end} {
        if(begin > end) throw std::out_of_range{"passed end"};
    }

    value_type operator * () const {
        if(begin >= end) throw std::out_of_range{"passed end"};
        auto info = Encoding::first_char_width(begin, end);
        if(info.result != enc::request_result::ok) {
            throw std::runtime_error{"getting mbc length"};
        }
        return {begin, begin + info.size};
    }

    auto& operator ++ () {
        if(begin >= end) throw std::out_of_range{"passed end"};
        auto info = Encoding::first_char_width(begin, end);
        if(info.result != enc::request_result::ok) {
            throw std::runtime_error{"getting mbc length"};
        }
        begin += info.size;
        return *this;
    }

    auto operator ++ (int) {
        auto prev = *this;
        ++(*this);
        return prev;
    }

    auto operator + (std::integral auto offset) const {
        auto offset0 = offset;
        auto it = begin;

        if(it < end) {
            while(offset-- > 0) {
                auto info = Encoding::first_char_width(it, end);
                if(info.result != std::codecvt_base::ok) throw std::runtime_error{"getting mbc length"};
                it += info.size;

                if(it == end) break;
            }
            return character_iterator<Encoding>{it, end};
        }

        throw std::out_of_range{
            "requested index: " + std::to_string(offset0)
        };
    }

    auto operator <=> (const character_iterator& other) const {
        return begin <=> other.begin;
    }

    bool operator == (const character_iterator& other) const = default;
    bool operator != (const character_iterator& other) const = default;
};

}