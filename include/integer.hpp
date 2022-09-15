#pragma once

#include "./if_satisfies.hpp"
#include "./__type/is_same_as.hpp"

template<unsigned Bits>
struct int_of_bits_type;

template<unsigned Bits>
struct uint_of_bits_type;

// 8
using int8 = signed char;
static_assert(sizeof(int8) == 1);

using uint8 = unsigned char;
static_assert(sizeof(uint8) == 1);

template<> struct int_of_bits_type<8>  { using type = int8;  };
template<> struct uint_of_bits_type<8> { using type = uint8; };

// 16
using int16 = signed short;
static_assert(sizeof(int16) == 2);

using uint16 = unsigned short;
static_assert(sizeof(uint16) == 2);

template<> struct int_of_bits_type<16>  { using type = int16;  };
template<> struct uint_of_bits_type<16> { using type = uint16; };

// 32
using int32 = signed int;
static_assert(sizeof(int32) == 4);

using uint32 = unsigned int;
static_assert(sizeof(uint32) == 4);

template<> struct int_of_bits_type<32>  { using type = int32;  };
template<> struct uint_of_bits_type<32> { using type = uint32; };

// 64

using int64 =
	if_satisfies<sizeof(long) == 8>::
	then<long>::
	otherwise<long long>;

static_assert(sizeof(int64) == 8);

using uint64 =
	if_satisfies<sizeof(unsigned long) == 8>::
	then<unsigned long>::
	otherwise<unsigned long long>;

static_assert(sizeof(uint64) == 8);

template<> struct int_of_bits_type<64>  { using type = int64;  };
template<> struct uint_of_bits_type<64> { using type = uint64; };

using nuint = typename uint_of_bits_type<sizeof(void*)*8>::type;
using nint = typename int_of_bits_type<sizeof(void*)*8>::type;

template<nuint Bits>
using int_of_bits = typename int_of_bits_type<Bits>::type;

template<nuint Bits>
using uint_of_bits = typename uint_of_bits_type<Bits>::type;

template<typename Type>
using uint_of_size_of = uint_of_bits<sizeof(Type) * 8>;

template<typename Type>
concept signed_integer = __type::is_same_as<
	typename int_of_bits_type<sizeof(Type)*8>::type,
	Type
>;

template<typename Type>
concept unsigned_integer = __type::is_same_as<
	typename uint_of_bits_type<sizeof(Type)*8>::type,
	Type
>;

template<typename Type>
concept integer = signed_integer<Type> || unsigned_integer<Type>;

namespace __type {

	template<typename Type>
	constexpr inline bool is_signed_integer = signed_integer<Type>;
	
	template<typename Type>
	constexpr inline bool is_unsigned_integer = unsigned_integer<Type>;

	template<typename Type>
	constexpr inline bool is_integer = integer<Type>;

}