#include <cstring>
#include <string>
#include <iostream>
#include <iterator>
#include <type_traits>
#include "include/cxx_util/mb/string.hpp"
#include "include/cxx_util/iterator.hpp"
#include <assert.h>

static_assert(enc::is_encoding_v<enc::utf8>, "");
static_assert(enc::is_encoding_v<enc::utf16>, "");
static_assert(enc::is_encoding_v<enc::ascii>, "");

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

void mb_string() {
    mb::utf8_string u8str = u8"Привет Hello 😇!";

    assert(u8str.size() == 6 + 1 + 5 + 1 + 1 + 1);
    assert(u8str.to_string_view().size() == 6*2 + 1 + 5 + 1 + 4 + 1);

    mb::ascii_string empty_ascii_str = "";
    assert(empty_ascii_str.size() == 0);

    mb::utf16_string u16_hello_world = u"Hello world!";

    assert(
        (u16_hello_world.template to_string<enc::ascii>()).size()
        ==
        std::strlen("Hello world!")*2
    );

    mb::utf8_string u8_hello_world = u8"Hello world!";

    assert(
        u8_hello_world.template to_string<enc::ascii>()
        ==
        "Hello world!"
    );
}

int main() {
    utf8_util();
    utf16_util();
    mb_string();
}