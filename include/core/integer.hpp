#pragma once

#include "primitive_integer.hpp"

template<unsigned Bits, template<auto> typename Switch>
struct integer_of_bits {
	static constexpr unsigned bits = Bits;
	using type = Switch<Bits>;
	type value{};

	integer_of_bits() = default;

	integer_of_bits(same_as<type> auto v)
		: value{ v } {}

	integer_of_bits(const integer_of_bits&) = default;

	explicit operator type& () { return value; }
	explicit operator const type& () const { return value; }

	auto& operator = (const integer_of_bits& v) {
		value = v.value;
		return *this;
	}

	integer_of_bits operator >> (type v) const {
		return { value >> v };
	}

	integer_of_bits operator << (type v) const {
		return { value >> v };
	}

	integer_of_bits operator & (type v) const {
		return { value & v };
	}

	integer_of_bits operator | (type v) const {
		return { value | v };
	}

	integer_of_bits operator | (integer_of_bits v) const {
		return { value | v.value };
	}
};

template<unsigned Bits>
using signed_integer_of_bits = integer_of_bits<Bits, primitive::int_of_bits>;

template<unsigned Bits>
using unsigned_integer_of_bits = integer_of_bits<Bits, primitive::uint_of_bits>;

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
using uint_native = unsigned_integer_of_bits<sizeof(void*)*8>;

template<typename T>
concept signed_integer = are_same<T, signed_integer_of_bits<T::bits>>;

template<typename T>
concept unsigned_integer = are_same<T, unsigned_integer_of_bits<T::bits>>;

template<typename T>
concept integer = signed_integer<T> || unsigned_integer<T>;