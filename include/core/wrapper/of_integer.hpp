#pragma once

#include "../integer.hpp"
#include "of.hpp"

namespace wrapper {

	template<integer I, typename S = void>
	using of_integer = wrapper::of<I, S>;
}

/*enum class is_signed {};

template<primitive::uint Bits, is_signed Signed>
struct integer_of_bits {
	static constexpr primitive::uint bits = Bits;
	static constexpr bool is_signed = (bool)Signed;

	using type =
		typename if_satisfy<is_signed>
		::template then<primitive::int_of_bits<Bits>>
		::template otherwise<primitive::uint_of_bits<Bits>>;
	
	type m_value{};

	constexpr integer_of_bits() = default;

	constexpr integer_of_bits(type v)
		: m_value{ v }
	{}

	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits(const integer_of_bits<Bits0, Signed>& v)
		: m_value{ v.m_value }
	{}

	constexpr explicit operator type& () { return m_value; }
	constexpr explicit operator const type& () const { return m_value; }

	// assignment
	// =
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator = (const integer_of_bits<Bits0, Signed>& v) {
		m_value = v.m_value;
		return *this;
	}

	// +=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator += (const integer_of_bits<Bits0, Signed>& v) {
		m_value += v.m_value;
		return *this;
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr auto& operator += (I v) const {
		m_value += v;
		return *this;
	}

	// -=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator -= (const integer_of_bits<Bits0, Signed>& v) {
		m_value -= v.m_value;
		return *this;
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr auto& operator -= (I v) {
		m_value -= v;
		return *this;
	}

	// *=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator *= (const integer_of_bits<Bits0, Signed>& v) {
		m_value *= v.m_value;
		return *this;
	}

	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr auto& operator *= (type v) {
		m_value *= v;
		return *this;
	}

	// /=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator /= (const integer_of_bits<Bits0, Signed>& v) {
		m_value /= v.m_value;
		return *this;
	}

	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr auto& operator /= (type v) {
		m_value /= v;
		return *this;
	}

	// |=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator |= (const integer_of_bits<Bits0, Signed>& v) {
		m_value |= v.m_value;
		return *this;
	}

	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr auto& operator |= (type v) {
		m_value |= v;
		return *this;
	}

	// &=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr auto& operator &= (const integer_of_bits<Bits0, Signed>& v) {
		m_value &= v.m_value;
		return *this;
	}

	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr auto& operator &= (type v) {
		m_value &= v;
		return *this;
	}

	// increment/decrement
	constexpr auto& operator ++ () {
		++m_value;
		return *this;
	}

	constexpr auto& operator -- () {
		--m_value;
		return *this;
	}

	constexpr integer_of_bits operator ++ (int) const {
		return { m_value++ };
	}

	constexpr integer_of_bits operator -- (int) const {
		return { m_value-- };
	}

	// +
	constexpr integer_of_bits operator + (type v) const {
		integer_of_bits copy{ m_value };
		copy.m_value += v;
		return copy;
	}

	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits operator + (integer_of_bits<Bits0, Signed> v) const {
		return *this + v.m_value;
	}

	// -
	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr integer_of_bits operator - (type v) const {
		integer_of_bits copy{ m_value };
		copy.m_value -= v;
		return copy;
	}

	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits operator - (integer_of_bits<Bits0, Signed> v) const {
		return *this - v.m_value;
	}

	// /
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits operator / (integer_of_bits<Bits0, Signed> v) const {
		return { m_value / v.m_value };
	}

	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr integer_of_bits operator / (type v) const {
		return { m_value / v};
	}

	// *
	constexpr integer_of_bits operator * (type v) const {
		integer_of_bits copy{ m_value };
		copy.m_value *= v;
		return copy;
	}

	// %
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr integer_of_bits operator % (integer_of_bits<Bits0, Signed> v) const {
		return { m_value % v.m_value };
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr integer_of_bits operator % (I v) const {
		return { m_value % v};
	}

	// >>
	constexpr integer_of_bits operator >> (type v) const {
		return { m_value >> v };
	}

	// <<
	constexpr integer_of_bits operator << (type v) const {
		return { m_value >> v };
	}

	// &
	constexpr integer_of_bits operator & (type v) const {
		return { m_value & v };
	}

	// |
	constexpr integer_of_bits operator | (type v) const {
		return { m_value | v };
	}

	constexpr integer_of_bits operator | (integer_of_bits v) const {
		return { m_value | v.m_value };
	}

	// ==
	constexpr bool operator == (integer_of_bits v) const {
		return m_value == v.m_value;
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr bool operator == (I v) const {
		return m_value == v;
	}

	// <
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr bool operator < (integer_of_bits<Bits0, Signed> v) const {
		return m_value < v.m_value;
	}

	constexpr bool operator < (type v) const {
		return m_value < v;
	}

	// >
	//template<primitive::integral I>
	//requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr bool operator > (type v) const {
		return m_value > v;
	}

	// <=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr bool operator <= (integer_of_bits<Bits0, Signed> v) const {
		return m_value <= v.m_value;
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr bool operator <= (I v) const {
		return m_value <= v;
	}

	// >=
	template<primitive::uint Bits0>
	requires(Bits0 <= Bits)
	constexpr bool operator >= (integer_of_bits<Bits0, Signed> v) const {
		return m_value >= v.m_value;
	}

	template<primitive::integral I>
	requires(is_signed == primitive::is_signed<I> && sizeof(I) * 8 <= bits)
	constexpr bool operator >= (I v) const {
		return m_value >= v;
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
using int32 = signed_integer_of_bits<32>;
using uint32 = unsigned_integer_of_bits<32>;

#if MAX_INT_BITS >= 64
using int64 = signed_integer_of_bits<64>;
using uint64 = unsigned_integer_of_bits<64>;
#endif

using uint = unsigned_integer_of_bits<MAX_INT_BITS>;

template<typename T>
concept signed_integer = types::are_same::for_types_of<T, signed_integer_of_bits<T::bits>>;

template<typename T>
concept unsigned_integer = types::are_same::for_types_of<T, unsigned_integer_of_bits<T::bits>>;

template<typename T>
concept integer = signed_integer<T> || unsigned_integer<T>;*/