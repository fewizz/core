#include <string>
#include <codecvt>
#include <iostream>
#include <iterator>
#include <type_traits>
#include "include/cxx_util/mbc_string.hpp"
#include "include/cxx_util/iterator.hpp"
#include "include/cxx_util/encoding.hpp"

using codec = std::codecvt_utf8_utf16<wchar_t>;
using encoding = util::utf8_encoding;

int main() {
    static_assert(util::is_input_iterator_v<util::mb::string_iterator<encoding>>, "not input iterator");
    static_assert(util::is_forward_iterator_v<util::mb::string_iterator<encoding>>, "not forward iterator");

    std::string str = "Привет";

    std::cout << str.size() << std::endl;

    util::mb::string_view<util::utf8_encoding> mbcstr{ str };

    std::cout << mbcstr.size() << std::endl;

    std::cout << mbcstr.to_string() << std::endl;

    for(auto ch : mbcstr)
        std::cout << ch << " " << std::flush;
    
    std::cout << std::endl;

    util::mb::string<util::utf16_encoding> u16 = mbcstr.convert<util::utf16_encoding>();

    std::cout << mbcstr.size() << std::endl;
}