#pragma once

#include <codecvt>

namespace enc {

struct size_retrieve_result {
    std::codecvt_base::result result;
    unsigned size;
};

}