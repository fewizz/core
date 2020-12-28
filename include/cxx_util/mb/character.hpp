#pragma once

#include <type_traits>
#include <utility>
#include "../encoding/encoding.hpp"
#include "../encoding/converter.hpp"

namespace mb {

template<enc::encoding Encoding>
struct character;

template<enc::encoding Encoding>
struct character_view;

namespace internal {

    template<class Parent, enc::encoding Encoding>
    struct character : Parent {
        using Parent::Parent;
        using char_type = typename Encoding::char_type;

        character(const Parent& parent) : Parent(parent){};
        character(Parent&& parent) : Parent(std::move(parent)){};
        character(char_type ch) : Parent(1, ch) {};

        auto data() const { return Parent::data();  }

        auto size() const { return Parent::size(); }

        bool operator == (std::integral auto ch) const {
            return this->size() == 1 && Parent::front() == ch;
        }

        bool operator != (std::integral auto ch) const {
            return !(*this == ch);
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

        std::basic_string<char_type> to_string() const {
            return { Parent::begin(), Parent::end() };
        }

        template<enc::encoding Encoding0>
        auto to_string() const {
            return convert <Encoding0>().to_string();
        }

        template<enc::encoding E = Encoding>
        operator std::enable_if_t<E::characters != enc::variable_length, char_type> () const {
            return Parent::front();
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