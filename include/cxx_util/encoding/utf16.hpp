#pragma once

#include <bit>

#include "unicode.hpp"
#include "codepoint.hpp"
#include "object/concepts.hpp"
#include "object/operations.hpp"

namespace u {

template<std::endian Endian = std::endian::native>
struct utf16;

using utf16be = utf16<std::endian::big>;
using utf16le = utf16<std::endian::little>;

template<std::endian E>
struct utf16_base {

using character_set = unicode;

struct decoder {
	std::size_t possible_size(u::atom_input_iterator auto it) {
		auto first = u::obj::read<uint16_t, E>(it);
	
		if(((first >= 0x0) && (first <= 0xD800)) || ((first >= 0xE000) && (first <= 0xFFFF)))
			return 1;
		
		return 2;
		/*auto second = u::obj::read<uint16_t, E>(it);
	
		uint16_t hs = first, ls = second;
	
		if(hs >= 0xD800 && hs <= 0xDBFF && ls >= 0xDC00 && ls <= 0xDFFF)
			return 2;
		
		return 0;*/
	}

	u::codepoint<character_set> size(u::atom_input_iterator auto it) {
		auto s = possible_size(it);

		auto hs = u::obj::read<uint32_t, E>(it);
		//if(!hs) return tl::unexpected{ request_error::unexpected_src_end };

		if(s == 1) {
			//res.codepoint = hs.value();
			return { hs };//res;
		};

		auto ls = u::obj::read<uint32_t, E>(*it);
		if(!ls) return tl::unexpected{ request_error::unexpected_src_end };

		res.codepoint = (((hs.value() & 0x3FF) << 10) | (ls.value() & 0x3FF)) + 0x10000;
		return res;
	}
};

/*template<u::iterator_of_bytes It>
static tl::expected<uint8_t, enc::request_error>
size(It begin, It end) {
	auto first_possible = u::read<uint16_t, Endian>(begin, end);
	if(!first_possible)
		return tl::unexpected {
			request_error::unexpected_src_end
		};

	auto first = first_possible.value();

	if(((first >= 0x0) && (first <= 0xD800)) || ((first >= 0xE000) && (first <= 0xFFFF)))
		return { 1 };
	
	auto second = u::read<uint16_t, Endian>(begin, end);
	if(!second) return tl::unexpected{ request_error::unexpected_src_end };

	uint16_t hs = first, ls = second.value();

	if(hs >= 0xD800 && hs <= 0xDBFF && ls >= 0xDC00 && ls <= 0xDFFF)
		return { 2 };
	
	return tl::unexpected{ request_error::invalid_input };
}

template<u::iterator_of_bytes It>
static constexpr tl::expected<enc::codepoint_read_result<unicode>, enc::request_error>
read(It begin, It end) {
	auto size_read = size(begin, end);
	if(!size_read) return tl::unexpected{ size_read.error() };

	codepoint_read_result<unicode> res;
	res.width = size_read.value();

	auto hs = u::read<uint64_t, Endian, 2>(begin, end);
	if(!hs) return tl::unexpected{ request_error::unexpected_src_end };

	if(size_read.value() == 1) {
		res.codepoint = hs.value();
		return res;
	};

	auto ls = u::read<uint64_t, Endian, 2>(begin, end);
	if(!ls) return tl::unexpected{ request_error::unexpected_src_end };

	res.codepoint = (((hs.value() & 0x3FF) << 10) | (ls.value() & 0x3FF)) + 0x10000;
	return res;
}

template<u::iterator_of_bytes It>
static constexpr
void write(codepoint<unicode> cp, It it, It end) {
	if((cp >= 0 && cp < 0xD800) || (cp >= 0xE000 && cp < 0x10000)) {
		u::write<Endian>(uint16_t(cp.value()), it, end);
		return;
	}

	uint32_t cp0 = cp.value() - 0x10000;

	uint32_t hs = 0xD800 + (cp0 >> 10);
	u::write<Endian>(hs, it, end);

	uint32_t ls = 0xD800 + (cp0 >> 10);
	u::write<Endian>(ls, it, end);
}*/

};

}