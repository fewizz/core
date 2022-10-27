#pragma once

#include "./if_satisfies.hpp"
#include "./__type/is_same_as.hpp"

template<unsigned Bits>
struct int_of_bytes_t;

template<unsigned Bits>
struct uint_of_bytes_t;

// 8
using int8 = signed char;
static_assert(sizeof(int8) == 1);

using uint8 = unsigned char;
static_assert(sizeof(uint8) == 1);

template<> struct int_of_bytes_t <1> { using type = int8;  };
template<> struct uint_of_bytes_t<1> { using type = uint8; };

// 16
using int16 = signed short;
static_assert(sizeof(int16) == 2);

using uint16 = unsigned short;
static_assert(sizeof(uint16) == 2);

template<> struct int_of_bytes_t <2> { using type = int16;  };
template<> struct uint_of_bytes_t<2> { using type = uint16; };

// 32
using int32 = signed int;
static_assert(sizeof(int32) == 4);

using uint32 = unsigned int;
static_assert(sizeof(uint32) == 4);

template<> struct int_of_bytes_t <4> { using type = int32;  };
template<> struct uint_of_bytes_t<4> { using type = uint32; };

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

template<> struct int_of_bytes_t <8> { using type = int64;  };
template<> struct uint_of_bytes_t<8> { using type = uint64; };

using nuint = typename uint_of_bytes_t<sizeof(void*)>::type;
using nint = typename int_of_bytes_t<sizeof(void*)>::type;

template<nuint Bytes>
using int_of_bytes = typename int_of_bytes_t<Bytes>::type;

template<nuint Bytes>
using uint_of_bytes = typename uint_of_bytes_t<Bytes>::type;

template<nuint Bits>
using int_of_bits = int_of_bytes<Bits/8>;

template<nuint Bits>
using uint_of_bits = uint_of_bytes<Bits/8>;

template<typename Type>
using int_of_size_of = int_of_bytes<sizeof(Type)>;

template<typename Type>
using uint_of_size_of = uint_of_bytes<sizeof(Type)>;

template<typename Type>
concept signed_integer = __type::is_same_as<
	int_of_size_of<Type>, Type
>;

template<typename Type>
concept unsigned_integer = __type::is_same_as<
	uint_of_size_of<Type>, Type
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

template<uint64 StatesCount>
/* returns
   uint8 for  0   < `StatesCount`<= 256,
   uint16 for 256 < `StatesCount`<= 65536,
   uint32 for 65536 < `StatesCount`< max unsigned int,
   uint64 otherwise.
*/
using uint_for_states_count = typename
	if_satisfies<(StatesCount <= (uint64{ 1 } << 8))>::template
	then<uint8>::template
	otherwise<
		typename if_satisfies<(StatesCount <= (uint64{ 1 } << 16))>::template
		then<uint16>::template
		otherwise<
			typename if_satisfies<(StatesCount <= (uint64{ 1 } << 32))>::template
			then<uint32>::template
			otherwise<
				uint64
			>
		>
	>;