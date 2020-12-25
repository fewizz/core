#include <string>
#include <codecvt>
#include <iostream>
#include <iterator>
#include <type_traits>
#include "include/cxx_util/multibyte_string.hpp"
#include "include/cxx_util/iterator.hpp"
#include "include/cxx_util/encoding.hpp"

using codec = std::codecvt_utf8_utf16<wchar_t>;
using encoding = util::enc::utf8;

int main() {
    static_assert(
        util::mb::is_string<util::mb::utf8_string>::value, "not string"
    );
    static_assert(util::is_encoding_v<util::enc::utf8>, "not encoding");
    static_assert(util::is_input_iterator_v<util::mb::string_iterator<encoding>>, "not input iterator");
    static_assert(util::is_forward_iterator_v<util::mb::string_iterator<encoding>>, "not forward iterator");

    std::string str = "Привет";

    std::cout << str.size() << std::endl;

    util::mb::utf8_string_view mbcstr{ str };

    std::cout << mbcstr.size() << std::endl;

    std::cout << mbcstr.to_string() << std::endl;

    for(auto ch : mbcstr)
        std::cout << ch << " " << std::flush;
    
    std::cout << std::endl;

    util::mb::utf16_string u16 = mbcstr.convert<util::enc::utf16>();

    std::cout << mbcstr.size() << std::endl;
    std::cout << mbcstr.template to_string<util::enc::utf8>() << std::endl;
}