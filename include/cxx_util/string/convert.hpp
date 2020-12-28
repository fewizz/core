#pragma once

#include <string>
#include <type_traits>
#include "../mb/string.hpp"

namespace util {

template<enc::encoding Encoding, class T>
auto convert_to_string(T&& value) {
    using D = std::decay_t<T>;

    if constexpr (std::is_same) {

    }
}


}