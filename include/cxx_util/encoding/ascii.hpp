#pragma once

#include <cinttypes>
#include "encoding.hpp"

namespace enc {

struct ascii {
	using codepoint_type = uint8_t;
	using character_set_type = ascii;

	static constexpr int code_unit_bits = 7;
	static constexpr int code_unit_size = 1;

	static constexpr tl::expected<uint8_t, request_error>
	size(auto begin, auto end) {
		return { 1 };
	}

	static constexpr tl::expected<codepoint_read_result<ascii>, request_error>
	read(u::byte_iterator auto begin, u::byte_iterator auto end) {
		codepoint_type possible = (uint8_t) *begin;

		if(possible >= 0x80) return tl::unexpected { request_error::invalid_input };

		return codepoint_read_result<ascii>{ possible, 1 };
	}

	static constexpr tl::expected<codepoint_write_result<ascii>, request_error>
	write(codepoint<ascii> cp, u::byte_iterator auto it, u::byte_iterator auto end) {
		*it = cp;
		return codepoint_write_result<ascii>{ 1 };
	}
};

}