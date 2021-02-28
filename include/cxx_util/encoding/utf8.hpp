#pragma once

#include <iterator>
#include <type_traits>
#include <utility>
#include <cinttypes>
#include <stdexcept>
#include <codecvt>
#include "request_error.hpp"
#include "../int.hpp"
#include "../bit.hpp"
#include <array>

#include "encoding.hpp"
#include "unicode.hpp"
#include <tl/expected.hpp>
#include "../byte_iterator.hpp"
#include "../iterator.hpp"

namespace enc {

struct utf8 {

using character_set_type = unicode;
static constexpr int code_unit_bits = 8;

static constexpr std::optional<uint8_t>
possible_size(std::byte byte) {
	if(u::equalsl<0>(byte)) return { 1 };
	if(u::equalsl<1, 1, 0>(byte)) return { 2 };
	if(u::equalsl<1, 1, 1, 0>(byte)) return { 3 };
	if(u::equalsl<1, 1, 1, 1, 0>(byte)) return { 4 };
	return {};
}

template<u::iterator_of_bytes It>
static constexpr tl::expected<uint8_t, enc::request_error>
size(It it, It end) {
	auto possible = possible_size(*it);
	if(!possible) return tl::unexpected{ enc::request_error::invalid_input };

	auto dist = std::distance(it, end);

	if(possible.value() > dist)
		return tl::unexpected{ enc::request_error::unexpected_src_end };
	
	for(int i = 1; i < std::min(decltype(dist)(possible.value()), dist); i++) {
		++it;

		if(not u::equalsl<1,0>( std::byte { *it }))
			return tl::unexpected{ enc::request_error::invalid_input };
	}

	return { possible.value() };
}

template<u::iterator_of_bytes It>
static constexpr tl::expected<codepoint_read_result<unicode>, enc::request_error>
read(It it, It end) {
	auto possible_size = size(it, end);
	if(!possible_size) return tl::unexpected{ possible_size.error() };

	auto width = possible_size.value();

	auto first = uint32_t( std::byte{ *it } );

	codepoint_read_result<unicode> res;
	res.width = width;
	
	if(width == 1) {
		res.codepoint = first;
		return res;
	}

	auto left_mask_size = width + 1;
	first &= 0xFF >> left_mask_size;

	unsigned first_offset = (6 * (width - 1));
	uint32_t resulting_cp = first << first_offset;

	for(unsigned i = 1; i < (unsigned)width; i++) {
		unsigned offset = 6 * ((width - 1) - i);
		++it;

		auto nth = uint32_t( std::byte{ *it } );
		resulting_cp |= (nth & 0b00111111) << offset;
	}
	
	res.codepoint = resulting_cp;
	return res;
}

template<u::iterator_of_bytes It> void
static constexpr write
(codepoint<unicode> cp, It it, It end) {
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

}