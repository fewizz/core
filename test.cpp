#include <string>
#include <codecvt>
#include <iostream>
#include "include/cxx_util/mbc_string.hpp"

int main() {
    std::string str = "Привет";
    
    std::cout << str.size() << std::endl;

    util::mb::string_view<std::codecvt_utf8<wchar_t>> mbcstr{ str };

    std::cout << mbcstr.size() << std::endl;
}