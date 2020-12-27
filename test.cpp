#include <cwchar>
#include <string>
#include <codecvt>
#include <iostream>
#include <iterator>
#include <type_traits>

#include <assert.h>

#include "include/cxx_util/mb/string.hpp"
#include "include/cxx_util/iterator.hpp"
#include "include/cxx_util/encoding.hpp"

#include "include/cxx_util/utf8.hpp"
#include "include/cxx_util/utf16.hpp"

void utf8_util() {
    char8_t buf[]{ 0xF0, 0x9F, 0x8C, 0x8A, 'a', 'b', 'c' };

    assert(4 == util::utf8::first_char_length(buf, buf + std::extent_v<decltype(buf)>).size);

    assert(0x1F30A == util::utf8::first_code_point(buf).codepoint);

    for(int i = 0; i < 4; i++)
        assert(std::codecvt_base::partial == util::utf8::first_code_point(buf, buf + i).result);

    char8_t invalid[]{ 0b10000000 };
    assert(std::codecvt_base::error == util::utf8::first_code_point(invalid).result);
}

void utf16_util() {
    char16_t buf[]{ 0xD83D, 0xDE02, 'a', 'b', 'c' };

    assert(2 == util::utf16::first_char_length(buf, buf + std::extent_v<decltype(buf)>).size);
    assert(0x1F602 == util::utf16::first_code_point(buf, buf + 2).codepoint);
}

int main() {
    utf8_util();
    utf16_util();
    
    mb::utf8_string str = u8"Привет Hello!";
    std::cout << "utf8 string size " << str.size() << std::endl;

    auto converted_string = str.template convert<enc::utf16>();
    std::cout << "converted to utf16 string size " << converted_string.size() << std::endl;

    auto converted_ascii = str.template convert<enc::ascii>();
    std::cout << "converted to ascii string size " << converted_ascii.size() << std::endl;
    std::cout << "ascii: " << converted_ascii.to_string() << std::endl;

    std::cout << "first utf-8 char size " << str[0].size() << std::endl;

    auto conv = str[0].template convert<enc::utf16>();

    std::cout << "first utf-16 char size " << conv.size() << std::endl;

    auto back = conv.template convert<enc::utf8>();

    std::cout << "back, first utf-8 char size " << back.size() << std::endl;



    /*util::reverted_codecvt<
        std::codecvt_utf8_utf16<wchar_t>
    > codec;

    std::mbstate_t state{};

    //std::cout << codec.length(state, buf, buf + 4, 2) << std::endl;

    static_assert(util::mb::is_string<util::mb::utf8_string>::value, "not string");
    static_assert(util::is_encoding_v<util::enc::utf8>, "not encoding");
    //static_assert(util::is_input_iterator_v<util::mb::string_iterator<encoding>>, "not input iterator");
    //static_assert(util::is_forward_iterator_v<util::mb::string_iterator<encoding>>, "not forward iterator");
    static_assert(std::is_same_v<util::mb::ascii_string::char_type, char>, "");

    std::u8string str = u8"Привет";

    std::cout << str.size() << std::endl;

    util::mb::utf8_string_view mbcstr{ str };

    std::cout << mbcstr.size() << std::endl;

    //std::cout << mbcstr.to_string() << std::endl;

    //for(auto ch : mbcstr)
    //    std::cout << ch << " " << std::flush;
    
    std::cout << std::endl;

    std::u16string str0 = u"😍";
    std::cout << util::enc::utf16<>::first_char_length(str0.data(), str0.data()+str0.size()) << std::endl;

    //util::mb::utf16_string u16 = mbcstr.convert<util::enc::utf16>();

    //std::cout << mbcstr.size() << std::endl;
    //std::cout << mbcstr.template to_string<util::enc::utf8>() << std::endl;*/
}