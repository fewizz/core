#pragma once

#include "meta/type.hpp"
#include "meta/are_types_same.hpp"

namespace primitive {

template<unsigned Bits>
struct int_of_bits_type;

template<unsigned Bits>
struct uint_of_bits_type;

// 8
using int8 = signed char;
static_assert(sizeof(int8) == 1);

using uint8 = unsigned char;
static_assert(sizeof(uint8) == 1);

template<> struct int_of_bits_type<8> : type_of<int8> {};
template<> struct uint_of_bits_type<8> : type_of<uint8> {};

// 16
using int16 = signed short;
static_assert(sizeof(int16) == 2);

using uint16 = unsigned short;
static_assert(sizeof(uint16) == 2);

template<> struct int_of_bits_type<16> : type_of<int16> {};
template<> struct uint_of_bits_type<16> : type_of<uint16> {};

// 32
using int32 = signed int;
static_assert(sizeof(int32) == 4);

using uint32 = unsigned int;
static_assert(sizeof(uint32) == 4);

template<> struct int_of_bits_type<32> : type_of<int32> {};
template<> struct uint_of_bits_type<32> : type_of<uint32> {};

// 64
using int64 = signed long;
static_assert(sizeof(int64) == 8);

using uint64 = unsigned long;
static_assert(sizeof(uint64) == 8);

template<> struct int_of_bits_type<64> : type_of<int64> {};
template<> struct uint_of_bits_type<64> : type_of<uint64> {};

using uint = typename uint_of_bits_type<sizeof(void*) * 8>::type;

template<uint Bits>
using int_of_bits = typename int_of_bits_type<Bits>::type;

template<uint Bits>
using uint_of_bits = typename uint_of_bits_type<Bits>::type;

template<typename T>
concept signed_integer = are_types_same<typename int_of_bits_type<sizeof(T)*8>::type, T>;

template<typename T>
concept unsigned_integer = are_types_same<typename uint_of_bits_type<sizeof(T)*8>::type, T>;

template<typename T>
concept integral = signed_integer<T> || unsigned_integer<T>;

template<integral T>
constexpr inline bool is_signed = signed_integer<T> ? true : false;

}