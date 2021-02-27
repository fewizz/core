#pragma once

#include <bits/stdint-uintn.h>
#include <cinttypes>
#include <concepts>
#include <iterator>
#include <ranges>
#include <cstdint>
#include <stdint.h>
#include <cstddef>

namespace u {

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
constexpr bool equalsr(Int mask, uint8_t bits, Int value) {
	return equals(mask, bits, value, 0);
}

template<class Int>
constexpr bool equalsl(Int mask, uint8_t bits, Int value) {
	return equals(mask, bits, value, sizeof(Int)*8 - bits);
}

template<uint8_t... Bits>
constexpr bool equalsr(auto value) {
	return equals<Bits...>(value, 0);
}

template<uint8_t... Bits>
constexpr bool equalsl(auto value) {
	return equals<Bits...>(value, sizeof(decltype(value))*8 - sizeof...(Bits));
}

}