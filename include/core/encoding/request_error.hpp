#pragma once

#include <bits/stdint-uintn.h>
#include <codecvt>
#include <optional>
#include <stdint.h>

namespace enc {


enum class request_error {
	preconditions = 1,
	unexpected_src_end = 2,
	invalid_input = 3
};

}