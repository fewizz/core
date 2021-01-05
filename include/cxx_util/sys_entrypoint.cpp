#include "sys_string.hpp"
#include <vector>

namespace util {
    using params_list = const std::vector<sys_string_view>&;
}

int entrypoint(util::params_list params);

#if defined(_WIN32) && defined(UNICODE)

extern "C" int wmain(int argc, wchar_t* argv[]) {
    return entrypoint({
        (char16_t**)argv, (char16_t**)argv + argc
    });
}

#else

int main(int argc, char* argv[]) {
    return entrypoint({
        (char8_t**)argv, (char8_t**)argv + argc
    });
}

#endif