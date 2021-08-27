#pragma once

#include <cinttypes>
#include <type_traits>
#include "iterator.hpp"
#include "convert.hpp"
#include "codepoint.hpp"

namespace u {

struct ascii {
	using codepoint_base_type = uint8_t;

	using decoder_type = u::bytes_to_object_converter<u::codepoint<ascii>>;
	using encoder_type = u::object_to_bytes_converter<u::codepoint<ascii>>;
};

}