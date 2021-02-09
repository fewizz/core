#pragma once

#include <bit>
#include <bits/stdint-uintn.h>
#include <iterator>
#include <locale>
#include <stdexcept>
#include <stdint.h>
#include <sys/types.h>
#include <utility>
#include <codecvt>
#include "request_error.hpp"
#include "../byte_iterator.hpp"
#include <tl/expected.hpp>

namespace util {

namespace utf16 {

template<util::byte_iterator It>
constexpr tl::expected<uint8_t, enc::request_error>
first_char_width(It it, It end) {
	auto size = std::distance(it, end);

	if(size <= 0) return tl::unexpected{ enc::request_error::preconditions };

	auto first = util::next<uint16_t>(it, end).value();

	if(((first >= 0x0) && (first <= 0xD800)) || ((first >= 0xE000) && (first <= 0xFFFF)))
		return { 1 };
	
	if(size == 1) { return tl::unexpected{ enc::request_error::unexpected_src_end }; }

	auto second = util::next<uint16_t>(it, end).value();

	uint16_t hs = first, ls = second;

	if(hs >= 0xD800 && hs <= 0xDBFF && ls >= 0xDC00 && ls <= 0xDFFF)
		return { 2 };
	
	return tl::unexpected{ enc::request_error::invalid_input };
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
	auto size = std::distance(it, end);

	if(size < 0) return tl::unexpected{ enc::request_error::preconditions };
	if(size == 0) return tl::unexpected{ enc::request_error::unexpected_src_end };

	auto size_read = first_char_width(it, end);
	if(!size_read) return size_read;

	uint64_t hs = util::next<u_int64_t, 2>(it, end).value();

	if(size_read.value() == 1) return { hs };

	uint64_t ls = util::next<u_int64_t, 2>(it, end).value();

	return {
		(((hs & 0x3FF) << 10) | (ls & 0x3FF)) + 0x10000
	};
}

constexpr tl::expected<uint64_t, enc::request_error>
first_codepoint(const std::ranges::range auto& range) {
	using It = decltype(std::begin(range));

	util::bytes_visitor_iterator<It, std::endian::big> begin{ std::begin(range) };
	util::bytes_visitor_iterator<It, std::endian::big> end{ std::end(range) };

	return first_codepoint(begin, end);
}

}

}