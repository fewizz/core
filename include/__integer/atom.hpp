#pragma once

#include "../if_satisfies.hpp"
#include "../__type/is_same_as.hpp"

// 'atom' integer always has size == 1

// signed integer type, representing single atom
using  int1a = signed char;
// unsigned integer type, representing single atom
using uint1a = unsigned char;

template<uint1a Bits>
struct int_of_atoms_t;

template<uint1a Bits>
struct uint_of_atoms_t;

template<> struct  int_of_atoms_t<1> { using type =  int1a; };
template<> struct uint_of_atoms_t<1> { using type = uint1a; };

// types with size == 2 atoms

// signed integer type, representing 2 atoms
using  int2a = signed short;
static_assert(sizeof(int2a) == 2);

// unsigned integer type, representing 2 atoms
using uint2a = unsigned short;
static_assert(sizeof(uint2a) == 2);

template<> struct  int_of_atoms_t<2> { using type =  int2a; };
template<> struct uint_of_atoms_t<2> { using type = uint2a; };

// types with size == 4 atoms

// signed integer type, representing 4 atoms
using  int4a = signed int;
static_assert(sizeof(int4a) == 4);

// unsigned integer type, representing 4 atoms
using uint4a = unsigned int;
static_assert(sizeof(uint4a) == 4);

template<> struct  int_of_atoms_t<4> { using type = int4a;  };
template<> struct uint_of_atoms_t<4> { using type = uint4a; };

// types with size == 8 atoms

// signed integer type, representing 8 atoms
using int8a =
	if_satisfies<sizeof(long) == 8>::
	then<long>::
	otherwise<long long>;

static_assert(sizeof(int8a) == 8);

// unsigned integer type, representing 8 atoms
using uint8a =
	if_satisfies<sizeof(unsigned long) == 8>::
	then<unsigned long>::
	otherwise<unsigned long long>;

static_assert(sizeof(uint8a) == 8);

template<> struct  int_of_atoms_t<8> { using type =  int8a; };
template<> struct uint_of_atoms_t<8> { using type = uint8a; };

template<uint8a Atoms>
using int_of_atoms  = typename  int_of_atoms_t<Atoms>::type;

template<uint1a Atoms>
using uint_of_atoms = typename uint_of_atoms_t<Atoms>::type;

// native signed integer type of sizeof(void*)) atoms
using  nint =  int_of_atoms<sizeof(void*)>;
// native unsigned integer type of sizeof(void*)) atoms
using nuint = uint_of_atoms<sizeof(void*)>;

constexpr inline uint1a bits_per_atom = [] {
	// now all v's bits are 1
	uint1a v = uint1a{ 0 } - uint1a{ 1 };
	// counting
	uint1a bits = 0;
	while(v > 0) {
		v >>= 1;
		++bits;
	}
	return bits;
}();

template<typename Type>
using  int_of_size_of =  int_of_atoms<sizeof(Type)>;

template<typename Type>
using uint_of_size_of = uint_of_atoms<sizeof(Type)>;

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