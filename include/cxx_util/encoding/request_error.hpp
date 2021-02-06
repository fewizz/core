#pragma once

#include <codecvt>
#include <stdint.h>

namespace enc {

enum class request_error {
    ok = 0,
    preconditions = 1,
    unexpected_src_end = 2,
    invalid_input = 3
};

}