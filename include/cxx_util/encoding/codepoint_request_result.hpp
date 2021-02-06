#pragma once

#include <codecvt>
#include <stdint.h>

namespace enc {

enum class request_status {
    ok = 0,
    preconditions = 1,
    unexpected_src_end = 2,
    invalid_input = 3
};


struct width_request_t {
    request_status status;
    uint8_t width;

    constexpr width_request_t(request_status status, uint8_t width)
        : status{ status }, width{ width } {}

    constexpr width_request_t(request_status status)
        : status{ status }, width{ 0 } {}

    constexpr operator uint8_t() { return width; }
};

struct codepoint_request_t {
    request_status status;
    uint64_t codepoint;
    uint8_t size;

    bool operator == (auto) = delete;
};

}