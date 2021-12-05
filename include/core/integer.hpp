#pragma once

#include "type/of.hpp"
#include "types/are_same.hpp"

template<unsigned Bits>
struct int_of_bits_type;

template<unsigned Bits>
struct uint_of_bits_type;

// 8
using int8 = signed char;
static_assert(sizeof(int8) == 1);

using uint8 = unsigned char;
static_assert(sizeof(uint8) == 1);

template<> struct int_of_bits_type<8> : type::of<int8> {};
template<> struct uint_of_bits_type<8> : type::of<uint8> {};

// 16
using int16 = signed short;
static_assert(sizeof(int16) == 2);

using uint16 = unsigned short;
static_assert(sizeof(uint16) == 2);

template<> struct int_of_bits_type<16> : type::of<int16> {};
template<> struct uint_of_bits_type<16> : type::of<uint16> {};

// 32
using int32 = signed int;
static_assert(sizeof(int32) == 4);

using uint32 = unsigned int;
static_assert(sizeof(uint32) == 4);

template<> struct int_of_bits_type<32> : type::of<int32> {};
template<> struct uint_of_bits_type<32> : type::of<uint32> {};

// 64
using int64 = signed long;
static_assert(sizeof(int64) == 8);

using uint64 = unsigned long;
static_assert(sizeof(uint64) == 8);

template<> struct int_of_bits_type<64> : type::of<int64> {};
template<> struct uint_of_bits_type<64> : type::of<uint64> {};

using nuint = typename uint_of_bits_type<sizeof(void*)*8>::type;

template<nuint Bits>
using int_of_bits = typename int_of_bits_type<Bits>::type;

template<nuint Bits>
using uint_of_bits = typename uint_of_bits_type<Bits>::type;

template<typename Type>
using uint_of_size_of = uint_of_bits<sizeof(Type) * 8>;

template<typename Type>
concept signed_integer = types::are_same::for_types_of<typename int_of_bits_type<sizeof(Type)*8>::type, Type>;

template<typename Type>
concept unsigned_integer = types::are_same::for_types_of<typename uint_of_bits_type<sizeof(Type)*8>::type, Type>;

template<typename Type>
concept integer = signed_integer<Type> || unsigned_integer<Type>;

struct is_signed_integer {
	static constexpr bool is_type_predicate = true;

	template<typename Type>
	static constexpr bool for_type_of = signed_integer<Type>;
};

struct is_unsigned_integer {
	static constexpr bool is_type_predicate = true;

	template<typename Type>
	static constexpr bool for_type_of = unsigned_integer<Type>;
};