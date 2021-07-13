#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cinttypes>
#include <stdexcept>
#include <tl/expected.hpp>
#include "codepoint.hpp"
#include "unicode.hpp"
#include "byte_iterator.hpp"
#include "object/concepts.hpp"

namespace u {

struct utf8 {

using character_set = unicode;

struct decoder_type {
	using size_type = std::size_t;
	
	size_type size(u::atom_input_iterator auto it) {
		if(u::equalsl<0>(*it)) return 1;
		if(u::equalsl<1, 1, 0>(*it)) return 2;
		if(u::equalsl<1, 1, 1, 0>(*it)) return 3;
		if(u::equalsl<1, 1, 1, 1, 0>(*it)) return 4;
		return -1;
	}

	template<class It> requires(
		u::iterator_of_bytes<std::remove_reference_t<It>>
	)
	u::codepoint<u::unicode> operator () (It&& it) {
		std::byte first_byte{ *it };

		auto size = size(first_byte).value();

		uint32_t result = std::to_integer<uint32_t>(first_byte);

		if(size == 1) {
			return { result };
		}

		uint8_t left_mask_size = size + 1;
		result &= 0xFF >> left_mask_size;

		uint8_t first_offset = (6 * (size - 1));
		result <<= first_offset;

		for(uint8_t byte_index = 1; byte_index < size; byte_index++) {
			++it;

			std::byte nth_byte = *it;

			uint8_t offset = 6 * ((size - 1) - byte_index);

			result |= (std::to_integer<uint32_t>(nth_byte) & 0b00111111) << offset;
		}
	
		return result;
	}

};

struct encoder_type {

template<u::iterator_of_bytes It> void
static convert(codepoint<unicode> cp, It&& it) {
	if(cp <= 0x7F) {
		*it++ = std::byte( cp.value() );
	}
	else if(cp <= 0x7FF) {
		*it++ = std::byte( ((cp.value() >> 6 ) & 0x7F) | 0b11000000 );
		*it++ = std::byte( ((cp.value() >> 0 ) & 0x3F) | 0x80 );
	}
	else if(cp <= 0xFFFF) {
		*it++ = std::byte( ((cp.value() >> 12) & 0xF ) | 0b11100000 );
		*it++ = std::byte( ((cp.value() >> 6 ) & 0x3F) | 0x80 );
		*it++ = std::byte( ((cp.value() >> 0 ) & 0x3F) | 0x80 );
	}
	else if(cp <= 0x10FFFF) {
		*it++ = std::byte( ((cp.value() >> 18) & 0x7 ) | 0b11110000 );
		*it++ = std::byte( ((cp.value() >> 12) & 0x3F) | 0x80 );
		*it++ = std::byte( ((cp.value() >> 6 ) & 0x3F) | 0x80 );
		*it++ = std::byte( ((cp.value() >> 0 ) & 0x3F) | 0x80 );
	}
}

};
};

}