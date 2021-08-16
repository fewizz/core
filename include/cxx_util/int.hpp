#pragma once

#include <cinttypes>
#include <type_traits>
#include <concepts>

namespace u {

namespace internal {

template<unsigned Size>
struct int_with_size;

template<> struct int_with_size<1> { using type = int8_t; };
template<> struct int_with_size<2> { using type = int16_t; };
template<> struct int_with_size<4> { using type = int32_t; };
template<> struct int_with_size<8> { using type = int64_t; };

}

template<unsigned Size>
using int_with_size = typename internal::int_with_size<Size>::type;

template<unsigned size>
using uint_with_size = std::make_unsigned_t<int_with_size<size>>;

template<typename T>
struct integral_like {
	T value;

	constexpr integral_like(T value)
		: value{value}
	{}

	constexpr operator const T& () const {
		return value;
	}
};

}