#pragma once

#include "codepoint.hpp"
#include "unicode.hpp"
#include "byte_iterator.hpp"
#include "object/bits.hpp"
#include "algorithms.hpp"

namespace u {

struct utf8 {

using character_set = unicode;

struct decoder {
	std::size_t possible_size(u::atom_input_iterator auto it) {
		u::obj::bits b{*it};

		if(u::ends_with(b, 0)) return 1;
		if(u::ends_with(b, 0, 1, 1)) return 2;
		if(u::ends_with(b, 0, 1, 1, 1)) return 3;
		if(u::ends_with(b, 0, 1, 1, 1, 1)) return 4;

		return 0;
	}

	void skip(u::atom_input_iterator auto& it) {
		std::advance(it, possible_size(it));
	}

	u::codepoint<u::unicode> operator () (u::atom_input_iterator auto it) {
		uint32_t byte0{ std::to_integer<uint32_t>(*it) };

		auto s = possible_size(it++);

		if(s == 1) {
			return { byte0 };
		}

		auto next = [&]() {
			return std::to_integer<uint32_t>(*it++) & 0b111111;
		};

		auto byte1 = next();
		if(s == 2) {
			return { ((byte0 & 0b11111) << 6) | byte1 };
		}

		auto byte2 = next();
		if(s == 3) {
			return { ((byte0 & 0b1111) << 12) | (byte1 << 6) | byte2 };
		}

		auto byte3 = next();
		if(s == 4) {
			return { ((byte0 & 0b111) << 18) | (byte1 << 12) | (byte2 << 6) | byte3 };
		}

		return { 0 };
	}

};

struct encoder {

void operator () (codepoint<unicode> cp, u::atom_output_iterator auto it) {
	if(cp <= 0x7Fu) {
		*it++ = std::byte( cp.value() );
	}
	else if(cp <= 0x7FFu) {
		*it++ = std::byte( ((cp.value() >> 6 ) & 0x7F) | 0b11000000 );
		*it++ = std::byte( ((cp.value() >> 0 ) & 0x3F) | 0x80 );
	}
	else if(cp <= 0xFFFFu) {
		*it++ = std::byte( ((cp.value() >> 12) & 0xF ) | 0b11100000 );
		*it++ = std::byte( ((cp.value() >> 6 ) & 0x3F) | 0x80 );
		*it++ = std::byte( ((cp.value() >> 0 ) & 0x3F) | 0x80 );
	}
	else if(cp <= 0x10FFFFu) {
		*it++ = std::byte( ((cp.value() >> 18) & 0x7 ) | 0b11110000 );
		*it++ = std::byte( ((cp.value() >> 12) & 0x3F) | 0x80 );
		*it++ = std::byte( ((cp.value() >> 6 ) & 0x3F) | 0x80 );
		*it++ = std::byte( ((cp.value() >> 0 ) & 0x3F) | 0x80 );
	}
}

};
};

}