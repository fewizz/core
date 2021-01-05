#pragma once

#include "common.hpp"

namespace mb {

namespace internal {

template<
    class Derived,
    enc::encoding E,
    class T = std::char_traits<typename E::char_type>
>
struct string_view :
internal::common<std::basic_string_view<typename E::char_type, T>, E> {
    using char_type = typename E::char_type;
    using string_view_type = std::basic_string_view<char_type, T>;
    using base_type = mb::internal::common<string_view_type, E>;
    using size_type = typename base_type::size_type;

    using base_type::base_type;

    Derived substr(size_type pos = 0, size_type count = base_type::npos) const {
        auto b = base_type::begin() + pos;
        auto e = count == base_type::npos ? base_type::end() : b + count;
        return { b, e };
    }
};

}

template<enc::encoding Encoding, class Traits>
struct basic_string_view;

template<enc::encoding E, class T>
struct basic_string_view : internal::string_view<basic_string_view<E, T>, E, T> {
    using internal::string_view<basic_string_view<E, T>, E, T>::string_view;
};

using ascii_string_view = mb::basic_string_view<enc::ascii>;

using utf8_string_view = mb::basic_string_view<enc::utf8>;

template<>
class basic_string_view<enc::utf16> :
public mb::internal::string_view<basic_string_view<enc::utf16>, enc::utf16> {
    using base = mb::internal::string_view<basic_string_view<enc::utf16>, enc::utf16>;
public:
    using base::base;

    operator std::wstring_view() const noexcept {
        return {(wchar_t*)data(), raw_size()};
    }
};

using utf16_string_view = mb::basic_string_view<enc::utf16>;

}