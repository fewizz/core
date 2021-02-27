#pragma once

#include <bit>
#include <iterator>
#include <locale>
#include <stdexcept>
#include <stdint.h>
#include <sys/types.h>
#include <utility>
#include <codecvt>
#include "encoding.hpp"
#include "unicode.hpp"
#include "request_error.hpp"
#include "../bytes.hpp"
#include <tl/expected.hpp>

namespace enc {

template<std::endian Endian = std::endian::native>
struct utf16_base;

using utf16be = utf16_base<std::endian::big>;
using utf16le = utf16_base<std::endian::little>;
using utf16 = utf16_base<std::endian::native>;

template<std::endian Endian>
struct utf16_base {

using character_set_type = unicode;

static constexpr tl::expected<uint8_t, enc::request_error>
size(
	u::input_iterator_of_type_convertible_to_byte auto begin,
	u::input_iterator_of_type_convertible_to_byte auto end
) {
	auto first_possible = u::next<Endian, uint16_t>(begin, end);
	if(!first_possible)
		return tl::unexpected {
			request_error::unexpected_src_end
		};

	auto first = first_possible.value();

	if(((first >= 0x0) && (first <= 0xD800)) || ((first >= 0xE000) && (first <= 0xFFFF)))
		return { 1 };
	
	auto second = u::next<Endian, uint16_t>(begin, end);
	if(!second) return tl::unexpected{ request_error::unexpected_src_end };

	uint16_t hs = first, ls = second.value();

	if(hs >= 0xD800 && hs <= 0xDBFF && ls >= 0xDC00 && ls <= 0xDFFF)
		return { 2 };
	
	return tl::unexpected{ request_error::invalid_input };
}

template<class It>
static constexpr tl::expected<enc::codepoint_read_result<unicode>, enc::request_error>
read(u::byte_iterator<It> begin, u::byte_iterator<It> end) {
	auto size_read = size(begin, end);
	if(!size_read) return tl::unexpected{ size_read.error() };

	codepoint_read_result<unicode> res;
	res.width = size_read.value();

	auto hs = u::next<Endian, uint64_t, 2>(begin, end);
	if(!hs) return tl::unexpected{ request_error::unexpected_src_end };

	if(size_read.value() == 1) {
		res.codepoint = hs.value();
		return res;
	};

	auto ls = u::next<Endian, uint64_t, 2>(begin, end);
	if(!ls) return tl::unexpected{ request_error::unexpected_src_end };

	res.codepoint = (((hs.value() & 0x3FF) << 10) | (ls.value() & 0x3FF)) + 0x10000;
	return res;
}

};

}