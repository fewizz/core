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

#include <tl/expected.hpp>
#include "../byte_iterator.hpp"

namespace util {

namespace utf8 {

constexpr std::optional<uint8_t> possible_size(std::byte byte) {
	if(equalsl<0>(byte)) return { 1 };
	if(equalsl<1, 1, 0>(byte)) return { 2 };
	if(equalsl<1, 1, 1, 0>(byte)) return { 3 };
	if(equalsl<1, 1, 1, 1, 0>(byte)) return { 4 };
	return {};
}

template<util::byte_iterator It>
constexpr tl::expected<uint8_t, enc::request_error>
size(It it, It end) {
	auto possible = possible_size(*it);
	if(!possible) return tl::unexpected{ enc::request_error::invalid_input };

	auto dist = std::distance(it, end);

	if(possible.value() > dist)
		return tl::unexpected{ enc::request_error::unexpected_src_end };
	
	for(int i = 1; i < std::min(decltype(dist)(possible.value()), dist); i++) {
		++it;

		if(not equalsl<1,0>(*it))
			return tl::unexpected{ enc::request_error::invalid_input };
	}

	return { possible.value() };
}

template<util::byte_iterator It>
constexpr tl::expected<enc::character, enc::request_error>
character(It it, It end) {
	auto possible_size = size(it, end);
	if(!possible_size) return tl::unexpected{ possible_size.error() };

	auto width = possible_size.value();

	auto first = uint64_t(*it);

	enc::character_builder cb;
	cb.width(width);
	
	if(width == 1)
		return { cb.codepoint(first) };

	auto left_mask_size = width + 1;
	first &= 0xFF >> left_mask_size;

	unsigned first_offset = (6 * (width - 1));
	uint64_t result = first << first_offset;

	for(unsigned i = 1; i < (unsigned)width; i++) {
		unsigned offset = 6 * ((width - 1) - i);
		++it;

		auto nth = uint64_t(*it);
		result |= (nth & 0b00111111) << offset;
	}

	return { cb.codepoint(result) };
}

}

}