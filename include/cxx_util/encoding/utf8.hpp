#pragma once

#include <bits/iterator_concepts.h>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <locale>
#include <stdint.h>
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
#include <tl/expected.hpp>
#include "../byte_iterator.hpp"

namespace enc {

struct utf8 {

using character_set_type = unicode;
static constexpr int code_unit_bits = 8;

static constexpr std::optional<uint8_t>
possible_size(std::byte byte) {
	if(util::equalsl<0>(byte)) return { 1 };
	if(util::equalsl<1, 1, 0>(byte)) return { 2 };
	if(util::equalsl<1, 1, 1, 0>(byte)) return { 3 };
	if(util::equalsl<1, 1, 1, 1, 0>(byte)) return { 4 };
	return {};
}

template<util::byte_iterator It>
static constexpr tl::expected<uint8_t, enc::request_error>
size(It it, It end) {
	auto possible = possible_size(*it);
	if(!possible) return tl::unexpected{ enc::request_error::invalid_input };

	auto dist = std::distance(it, end);

	if(possible.value() > dist)
		return tl::unexpected{ enc::request_error::unexpected_src_end };
	
	for(int i = 1; i < std::min(decltype(dist)(possible.value()), dist); i++) {
		++it;

		if(not util::equalsl<1,0>(*it))
			return tl::unexpected{ enc::request_error::invalid_input };
	}

	return { possible.value() };
}

template<util::byte_iterator It>
static constexpr tl::expected<codepoint_parse_result<unicode>, enc::request_error>
read(It it, It end) {
	auto possible_size = size(it, end);
	if(!possible_size) return tl::unexpected{ possible_size.error() };

	auto width = possible_size.value();

	auto first = uint32_t(*it);

	codepoint_parse_result<unicode> res;
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

		auto nth = uint32_t(*it);
		resulting_cp |= (nth & 0b00111111) << offset;
	}
	
	res.codepoint = resulting_cp;
	return res;
}

template<util::byte_iterator It> void
write(codepoint<unicode> cp, It it, It end) {
	if(cp <= 0x7F) {
		it = 
	}
}

};

}