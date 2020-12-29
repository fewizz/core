#pragma once

#include <codecvt>

namespace enc {

enum class request_result {
    ok = 0,
    preconditions = 1,
    unexpected_src_end = 2,
    invalid_input = 3
};


struct size_request_info {
    request_result result;
    unsigned size;
};

struct codepoint_request_info {
    request_result result;
    uint64_t codepoint;
    unsigned size;
    //request_result width_request_result;
};

}