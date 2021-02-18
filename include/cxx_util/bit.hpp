#pragma once

#include <bits/stdint-uintn.h>
#include <cinttypes>
#include <concepts>
#include <iterator>
#include <ranges>
#include <cstdint>
#include <stdint.h>
#include <cstddef>

namespace util {

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
constexpr bool equals(auto val, uint8_t position) {
	check_bits<Bits...>();
	using Int = decltype(val);

	Int mask{ 0 };
	for(uint8_t bit : { Bits... }) {
		mask <<= 1;
		mask |= Int{ bit };
	}
	
	return equals(mask, sizeof...(Bits), val, position);
}

template<class Int>
constexpr bool equalsr(Int mask, uint8_t bits, Int val) {
	return equals(mask, bits, val, 0);
}

template<class Int>
constexpr bool equalsl(Int mask, uint8_t bits, Int val) {
	return equals(mask, bits, val, sizeof(Int)*8 - bits);
}

template<uint8_t... Bits>
constexpr bool equalsr(auto val) {
	return equals<Bits...>(val, 0);
}

template<uint8_t... Bits>
constexpr bool equalsl(auto val) {
	return equals<Bits...>(val, sizeof(decltype(val))*8 - sizeof...(Bits));
}

template<class Int>
constexpr std::byte read_byte(Int val, unsigned index) {
	return std::byte{ uint8_t( val >> ( index * 8 ) & 0xFF) };
}

template<class Int>
constexpr void write_byte(std::byte b, Int& val, unsigned index) {
	std::byte{ uint8_t( val >> ( index * 8 ) & 0xFF) };
}

template<class Int>
constexpr Int change_endianness(Int val) {
	unsigned size = sizeof(Int);

	for(unsigned byte = 0; byte < size / 2; byte++) {
		unsigned
			start_bit0 = byte * 8,
			start_bit1 = (size - byte - 1) * 8;

		Int byte_val0 = ( val >> start_bit0 ) & 0xFF;
		Int byte_val1 = ( val >> start_bit1 ) & 0xFF;

		val &= ~(0xFF << start_bit0);
		val &= ~(0xFF << start_bit1);

		val |= byte_val1 << start_bit0;
		val |= byte_val0 << start_bit1;
	}

	return val;
}

}