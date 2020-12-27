#pragma once

#include <codecvt>

namespace enc {

struct codepoint_retrieve_result {
    std::codecvt_base::result result;
    uint64_t codepoint;
    unsigned size_result;
};

}