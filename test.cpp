#include <string>
#include <codecvt>
#include <iostream>
#include "include/cxx_util/mbc_string_view.hpp"

int main() {
    std::string str = "Привет";
    std::cout << str.size() << std::endl;

    util::mbc_string_view<std::codecvt_utf8<wchar_t>> mbcstr{str};

    std::cout << mbcstr.size() << std::endl;
}