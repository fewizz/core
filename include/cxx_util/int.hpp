#pragma once

#include <cinttypes>
#include <type_traits>

namespace util {

template<unsigned size>
struct int_with_size;

template<> struct int_with_size<1> { using type = int8_t; };
template<> struct int_with_size<2> { using type = int16_t; };
template<> struct int_with_size<4> { using type = int32_t; };
template<> struct int_with_size<8> { using type = int64_t; };

template<unsigned size>
using int_with_size_t = typename int_with_size<size>::type;

template<unsigned size>
using uint_with_size_t = std::make_unsigned_t<int_with_size_t<size>>;

}