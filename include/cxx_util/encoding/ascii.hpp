#pragma once

#include <bits/stdint-uintn.h>
#include <cinttypes>
#include "encoding.hpp"
#include "../iterator.hpp"

namespace enc {

struct ascii : char_set_encoding<ascii, uint8_t> {
	using character_set_type = ascii;
	using codepoint_type = uint8_t;

	static constexpr int code_unit_bits = 7;
	static constexpr int code_unit_size = 1;
};

}