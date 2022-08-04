#pragma once

#include "span.hpp"

struct memory : span<uint8> {
	using base_type = span<uint8>;
	using base_type::base_type;
};