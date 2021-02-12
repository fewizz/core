#pragma once

#include "../encoding/encoding.hpp"

namespace mb {

namespace internal {
    template<class Base, enc::encoding Encoding>
    struct common;
}

template<
    enc::encoding Encoding,
    class Traits = std::char_traits<typename Encoding::char_type>,
    class Allocator = std::allocator<typename Encoding::char_type>
>
struct basic_string;

template<enc::encoding Encoding, class Traits = std::char_traits<typename Encoding::char_type>>
struct basic_string_view;

}