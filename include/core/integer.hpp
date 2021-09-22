#pragma once

#include "primitive_integer.hpp"
#include "meta/if_satisfying.hpp"

enum class is_signed {};

template<primitive::uint Bits, is_signed Signed>
struct integer_of_bits {
	static constexpr primitive::uint bits = Bits;
	static constexpr bool is_signed = (bool)Signed;
	static constexpr primitive::uint bytes = bits * 8;

	using type =
		typename if_satisfying<is_signed>
		::template then<primitive::int_of_bits<Bits>>
		::template otherwise<primitive::uint_of_bits<Bits>>;
	
	type value{};

	constexpr integer_of_bits() = default;

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) <= bytes)
	constexpr integer_of_bits(I v)
		: value{ v } {}

	constexpr integer_of_bits(const integer_of_bits&) = default;

	constexpr explicit operator type& () { return value; }
	constexpr explicit operator const type& () const { return value; }

	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator = (const integer_of_bits<Bits0, Signed>& v) {
		value = v.value;
		return *this;
	}

	// +
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits operator + (integer_of_bits<Bits0, Signed> v) const {
		return { value + v.value };
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) <= bytes)
	constexpr integer_of_bits operator + (I v) const {
		return { value + v};
	}

	// -
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits operator - (integer_of_bits<Bits0, Signed> v) const {
		return { value - v.value };
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) <= bytes)
	constexpr integer_of_bits operator - (I v) const {
		return { value - v};
	}

	// >>
	constexpr integer_of_bits operator >> (type v) const {
		return { value >> v };
	}

	// <<
	constexpr integer_of_bits operator << (type v) const {
		return { value >> v };
	}

	// &
	constexpr integer_of_bits operator & (type v) const {
		return { value & v };
	}

	// |
	constexpr integer_of_bits operator | (type v) const {
		return { value | v };
	}

	constexpr integer_of_bits operator | (integer_of_bits v) const {
		return { value | v.value };
	}

	// ==
	constexpr bool operator == (integer_of_bits v) const {
		return value == v.value;
	}

	// >
	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) <= bytes)
	constexpr bool operator > (I v) const {
		return value > v.value;
	}
};

template<unsigned Bits>
using signed_integer_of_bits = integer_of_bits<Bits, is_signed{ true }>;

template<unsigned Bits>
using unsigned_integer_of_bits = integer_of_bits<Bits, is_signed{ false }>;

template<typename T>
using signed_integer_of_size_of = signed_integer_of_bits<sizeof(T)*8>;

template<typename T>
using unsigned_integer_of_size_of = unsigned_integer_of_bits<sizeof(T)*8>;

using int8 = signed_integer_of_bits<8>;
using uint8 = unsigned_integer_of_bits<8>;
using int16 = signed_integer_of_bits<16>;
using uint16 = unsigned_integer_of_bits<16>;
using sint32 = signed_integer_of_bits<32>;
using uint32 = unsigned_integer_of_bits<32>;
using int64 = signed_integer_of_bits<64>;
using uint64 = unsigned_integer_of_bits<64>;
using uint = unsigned_integer_of_bits<sizeof(void*)*8>;

template<typename T>
concept signed_integer = are_same<T, signed_integer_of_bits<T::bits>>;

template<typename T>
concept unsigned_integer = are_same<T, unsigned_integer_of_bits<T::bits>>;

template<typename T>
concept integer = signed_integer<T> || unsigned_integer<T>;