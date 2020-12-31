#pragma once

#include <compare>
#include <type_traits>
#include <utility>
#include "../encoding/encoding.hpp"
#include "../encoding/convert.hpp"

namespace mb {

template<enc::encoding Encoding>
struct character;

template<enc::encoding Encoding>
struct character_view;

namespace internal {

    template<class Base, enc::encoding Encoding>
    struct character : Base {
        using Base::Base;
        using char_type = typename Encoding::char_type;

        character(const Base& parent) : Base(parent){};
        character(Base&& parent) : Base(std::move(parent)){};
        character(char_type ch) : Base(1, ch) {};

        using Base::data;
        using Base::size;
        
        auto operator <=> (const character& other) const {
            return
                Encoding::first_codepoint(data(), data() + size()).codepoint
                <=>
                Encoding::first_codepoint(other.data(), other.data() + other.size()).codepoint
            ;
        }

        bool operator == (std::integral auto ch) const {
            return
                this->size() == 1
                &&
                Encoding::first_codepoint(data(), data() + size()).codepoint == ch
            ;
        }

        template<enc::encoding Encoding0>
        mb::character<Encoding0> convert() const {
            auto from = util::template from<Encoding>(data(), data() + size());

            if(from.template to_always_noconv<Encoding0>()) {
                return {data(), data() + size()};
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

        template<enc::encoding Encoding0 = Encoding>
        mb::character<Encoding0> to_string() const {
            return convert<Encoding0>();
        }

        template<std::integral CharT = char_type>
        std::basic_string<CharT> to_string() const {
            return { (CharT*)Base::data(), (CharT*)Base::data() + Base::size() };
        }

        //template<std::>
        auto to_string_view() const {
            return to_string_view<char_type>();
        }

        template<enc::encoding E = Encoding>
        operator std::enable_if_t<E::characters != enc::variable_width, char_type> () const {
            return Base::front();
        }
    };
}

template<enc::encoding Encoding>
struct character_view : internal::character<std::basic_string_view<typename Encoding::char_type>, Encoding> {
    using internal::character<std::basic_string_view<typename Encoding::char_type>, Encoding>::character;
};

template<enc::encoding Encoding>
struct character : internal::character<std::basic_string<typename Encoding::char_type>, Encoding> {
    using char_type = typename Encoding::char_type;
    using string_type = std::basic_string<char_type>;
    using base_type = internal::character<string_type, Encoding>;

    using base_type::base_type;

    operator character_view<Encoding> () const {
        return { string_type::operator std::template basic_string_view<char_type>() };
    }
};

}