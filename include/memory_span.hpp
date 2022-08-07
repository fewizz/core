#pragma once

#include "span.hpp"

struct memory_span : span<uint8> {
	using base_type = span<uint8>;
	using base_type::base_type;
};