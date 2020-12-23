#include <string>
#include <codecvt>
#include <iostream>
#include <wchar.h>
#include "include/cxx_util/mbc_string.hpp"

int main() {
    std::string str = "Привет";

    std::cout << str.size() << std::endl;

    util::mb::string_view<std::codecvt_utf8<wchar_t>> mbcstr{ str };

    std::cout << mbcstr.size() << std::endl;

    std::cout << mbcstr.to_string() << std::endl;

    //auto it = mbcstr.begin();
    //++it;

    //std::cout << util::mb::string_view<std::codecvt_utf8<wchar_t>>{ it, mbcstr.end() }.to_string() << std::endl;

    for(auto ch : mbcstr) {
        std::cout << "+" << std::endl;
        std::cout << ch << std::endl;
    }
}