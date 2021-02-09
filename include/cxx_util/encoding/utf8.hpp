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

constexpr tl::expected<uint8_t, enc::request_error> possible_char_width(std::byte byte) {
	if(equalsl<0>(byte)) return { 1 };
	if(equalsl<1, 1, 0>(byte)) return { 2 };
	if(equalsl<1, 1, 1, 0>(byte)) return { 3 };
	if(equalsl<1, 1, 1, 1, 0>(byte)) return { 4 };
	return tl::unexpected{ enc::request_error::invalid_input };
}

template<util::byte_iterator It>
constexpr tl::expected<uint8_t, enc::request_error>
first_char_width(It it, It end) {
	auto size = std::distance(it, end);
	if(size <= 0) return tl::unexpected{ enc::request_error::preconditions };

	auto possible = possible_char_width(*it);

	if(!possible) return possible;

	if(possible.value() > size)
		return tl::unexpected{ enc::request_error::unexpected_src_end };
	
	for(int i = 1; i < std::min(decltype(size)(possible.value()), size); i++) {
		++it;

		if(not equalsl<1,0>(*it))
			return tl::unexpected{ enc::request_error::invalid_input };
	}

	return possible;
}

constexpr tl::expected<uint8_t, enc::request_error>
first_char_width(const std::ranges::range auto& range) {
	util::bytes_visitor_iterator begin{ std::begin(range) };
	util::bytes_visitor_iterator end{ std::end(range) };

	return first_char_width(begin, end);
}

template<util::byte_iterator It>
constexpr tl::expected<uint64_t, enc::request_error>
first_codepoint(It it, It end) {
	auto possible_width = first_char_width(it, end);
	if(!possible_width) return possible_width;

	auto width = possible_width.value();

	auto first = uint64_t(*it);

	if(width == 1) return { first };

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

	return result;
}

constexpr tl::expected<uint64_t, enc::request_error>
first_codepoint(const std::ranges::range auto& range) {
	util::bytes_visitor_iterator begin{ std::begin(range) };
	util::bytes_visitor_iterator end{ std::end(range) };

	return first_codepoint(begin, end);
}

}

}