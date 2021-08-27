#pragma once

#include <cinttypes>
#include <type_traits>
#include <concepts>

namespace internal {

template<std::size_t Size>
struct int_with_size;

template<> struct int_with_size<1> { using type = int8_t; };
template<> struct int_with_size<2> { using type = int16_t; };
template<> struct int_with_size<4> { using type = int32_t; };
template<> struct int_with_size<8> { using type = int64_t; };

}

template<unsigned Size>
using int_with_size = typename internal::int_with_size<Size>::type;

template<typename T>
using int_with_size_of = int_with_size<sizeof(T)>;

template<unsigned size>
using uint_with_size = std::make_unsigned_t<int_with_size<size>>;

template<typename T>
using uint_with_size_of = uint_with_size<sizeof(T)>;

template<typename T>
concept integer = std::integral<T>;

template<typename T>
concept unsigned_integer = integer<T> && std::is_unsigned_v<T>;