#pragma once

#include <cinttypes>
#include <concepts>
#include <iterator>
#include <ranges>
#include <cstdint>
#include <stdint.h>
#include <cstddef>
#include <type_traits>
#include "interface/iterator.hpp"
#include "iterator.hpp"

namespace u {

template<class O> requires(std::is_trivial_v<O>)
class bit_reference {
	O& m_o;
	unsigned m_index;

public:
	constexpr bit_reference(O& o, unsigned index) : m_o{ o }, m_index{ index } {}

	bool get() const {
		uint8_t* bytes = (uint8_t*)&m_o;

		return (
			bytes[m_index >> 3] >> (m_index & 0b111)
		) & 1;
	}

	void set(bool value) {
		uint8_t* bytes = (uint8_t*)&m_o;
		bytes[m_index >> 3] |= uint8_t(value) << (m_index & 0b111);
	}
};


template<class It>
class bit_iterator
: u::iterator<
	u::iter_concept_t<It>,
	bit_iterator<It>,
	u::value_type<bit_reference<std::iter_value_t<It>>>
> {
	It m_it;
	std::size_t m_bit_index = 0;

public:
	using base_value_type = std::iter_value_t<It>;

	using value_type = bit_reference<std::iter_value_t<It>>;

	static constexpr std::size_t
		base_value_type_size = sizeof(base_value_type);

	value_type operator * () const {
		return bit_reference{ *m_it, m_bit_index };
	}

	value_type operator ++ () {
		++m_byte_index;
		if(m_byte_index == base_value_type_size) {
			++m_it;
			m_byte_index = 0;
		}
		return *this;
	}
	using base_type::operator ++ ;

};


template<uint8_t HeadBit, uint8_t... Bits>
constexpr void check_bits() {
	static_assert(HeadBit <= 1, "bit can't be greater than 1");
	if constexpr(sizeof...(Bits) > 0) check_bits<Bits...>();
}

template<class Int>
constexpr bool equals(Int mask, uint8_t bits, Int val, uint8_t position) {
	Int mask0 = ~(Int(-1) << bits);
	mask0 <<= position;
	return (val & mask0) == (mask << position);
}

template<uint8_t... Bits>
constexpr bool equals(auto value, uint8_t position) {
	check_bits<Bits...>();
	using Int = decltype(value);

	Int mask { 0 };
	for(uint8_t bit : { Bits... }) {
		mask <<= 1;
		mask |= Int{ bit };
	}
	
	return equals(mask, sizeof...(Bits), value, position);
}

template<class Int>
constexpr bool equals_right(Int mask, uint8_t bits, Int value) {
	return equals(mask, bits, value, 0);
}

template<class Int>
constexpr bool equals_left(Int mask, uint8_t bits, Int value) {
	return equals(mask, bits, value, sizeof(Int)*8 - bits);
}

template<uint8_t... Bits>
constexpr bool equals_right(auto value) {
	return equals<Bits...>(value, 0);
}

template<uint8_t... Bits>
constexpr bool equals_left(auto value) {
	return equals<Bits...>(value, sizeof(decltype(value))*8 - sizeof...(Bits));
}

}