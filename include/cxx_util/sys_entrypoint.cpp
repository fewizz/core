#include "sys_string.hpp"

extern "C" int entrypoint(util::params_list params);

#ifdef _WIN32
extern "C" int wmain(int argc, wchar_t* argv[]) {
    return entrypoint({
        (char16_t**)argv, (char16_t**)argv + argc
    });
}

#else

int main(int argc, char* argv[]) {
    return entrypoint({
        (char8_t**)argv, argv + argc
    });
}

#endif