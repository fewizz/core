#pragma once

#include "bit.hpp"
#include <bit>

namespace u {

template<std::endian E = std::endian::native>
constexpr std::byte read_byte(auto from, unsigned index)
requires( std::integral<decltype(from)> ) {
	auto byte_shift = E == std::endian::big ?
		sizeof(from) - index - 1
		: index;
	auto bit_shift = byte_shift * 8;

	return std::byte {
		uint8_t( from >> bit_shift )
	};
}

template<std::endian E = std::endian::native>
constexpr void write_byte(std::byte value, std::integral auto& to, unsigned index) {
	auto byte_shift = E == std::endian::big ?
		sizeof(to) - index - 1
		: index;
	auto bit_shift = byte_shift * 8;

	std::decay_t<decltype(to)> clear_mask = ~( 0xFF << bit_shift );
	to &= clear_mask;
	to |= std::decay_t<decltype(to)>(value) << bit_shift;
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