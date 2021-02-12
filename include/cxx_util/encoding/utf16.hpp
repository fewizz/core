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
size(It begin, It end) {
	auto first_possible = util::next<uint16_t>(begin, end);
	if(!first_possible)
		return tl::unexpected {
			enc::request_error::unexpected_src_end
		};

	auto first = first_possible.value();

	if(((first >= 0x0) && (first <= 0xD800)) || ((first >= 0xE000) && (first <= 0xFFFF)))
		return { 1 };
	
	auto second = util::next<uint16_t>(begin, end);
	if(!second) return tl::unexpected{ enc::request_error::unexpected_src_end };

	uint16_t hs = first, ls = second.value();

	if(hs >= 0xD800 && hs <= 0xDBFF && ls >= 0xDC00 && ls <= 0xDFFF)
		return { 2 };
	
	return tl::unexpected{ enc::request_error::invalid_input };
}

template<util::byte_iterator It>
constexpr tl::expected<enc::character, enc::request_error>
character(It begin, It end) {
	auto size_read = size(begin, end);
	if(!size_read) return tl::unexpected{ size_read.error() };

	enc::character_builder cb;
	cb.width(size_read.value());

	auto hs = util::next<uint64_t, 2>(begin, end);
	if(!hs) return tl::unexpected{ enc::request_error::unexpected_src_end };

	if(size_read.value() == 1) return { cb.codepoint(hs.value()) };

	auto ls = util::next<uint64_t, 2>(begin, end);
	if(!ls) return tl::unexpected{ enc::request_error::unexpected_src_end };

	return {
		cb.codepoint(
			(((hs.value() & 0x3FF) << 10) | (ls.value() & 0x3FF)) + 0x10000
		)
	};
}

}

}