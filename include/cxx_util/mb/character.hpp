#pragma once

#include <utility>
#include "../encoding.hpp"
#include "../converter.hpp"

namespace mb {

namespace internal {
    template<class Parent, class Encoding>
    struct _character;
}

template<class Encoding>
struct character;

template<class Encoding>
struct character_view;

namespace internal {

    template<class Parent, class Encoding>
    struct _character : Parent {
        using Parent::Parent;
        using char_type = typename Encoding::char_type;

        _character(const Parent& parent) : Parent(parent){};
        _character(Parent&& parent) : Parent(std::move(parent)){};
        _character(char_type ch) : Parent(1, ch) {};

        auto data() const { return Parent::data();  }

        auto size() const { return Parent::size(); }

        bool operator == (std::integral auto ch) const {
            return this->size() == 1 && Parent::front() == ch;
        }

        bool operator != (std::integral auto ch) const {
            return !(*this == ch);
        }

        template<class Encoding0>
        character<Encoding0> convert() const {
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
    using char_type = typename Encoding::char_type;
    using string_type = std::basic_string<char_type>;
    using base_type = internal::_character<string_type, Encoding>;

    using base_type::base_type;

    operator character_view<Encoding> () const {
        return { string_type::operator std::template basic_string_view<char_type>() };
    }
};

}