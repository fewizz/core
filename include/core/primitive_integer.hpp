#pragma once

#include "meta/conditional.hpp"
#include "meta/same.hpp"
#include "meta/type.hpp"

namespace primitive {

template<unsigned Bits>
struct int_of_bits_type;

template<typename T>
struct make_unsigned_type;

// 8
using int8 = signed char;
static_assert(sizeof(int8) == 1);

using uint8 = unsigned char;
static_assert(sizeof(uint8) == 1);

template<> struct int_of_bits_type<8> : type::of<int8> {};
template<> struct make_unsigned_type<int8> : type::of<uint8> {};

// 16
using int16 = signed short;
static_assert(sizeof(int16) == 2);

using uint16 = unsigned short;
static_assert(sizeof(uint16) == 2);

template<> struct int_of_bits_type<16> : type::of<int16> {};
template<> struct make_unsigned_type<int16> : type::of<uint16> {};

// 32
using int32 = signed int;
static_assert(sizeof(int32) == 4);

using uint32 = unsigned int;
static_assert(sizeof(uint32) == 4);

template<> struct int_of_bits_type<32> : type::of<int32> {};
template<> struct make_unsigned_type<int32> : type::of<uint32> {};

// 64
using int64 = signed long;
static_assert(sizeof(int64) == 8);

using uint64 = unsigned long;
static_assert(sizeof(uint64) == 8);

template<> struct int_of_bits_type<64> : type::of<int64> {};
template<> struct make_unsigned_type<int64> : type::of<uint64> {};

template<typename T>
using make_unsigned = make_unsigned_type<T>;

template<unsigned Bits>
using int_of_bits = typename int_of_bits_type<Bits>::type;

template<unsigned Bits>
using uint_of_bits = typename make_unsigned<int_of_bits<Bits>>::type;

using uint_native = uint_of_bits<sizeof(void*) * 8>;

}