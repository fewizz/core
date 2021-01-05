#include <initializer_list>
#include <sstream>
#include <string>
#include <iostream>
#include <iterator>
#include <string_view>
#include <type_traits>
#include "include/cxx_util/mb/string.hpp"
#include "include/cxx_util/mb/string_view.hpp"
#include "include/cxx_util/iterator.hpp"
#include <assert.h>
#include "include/cxx_util/containers/join.hpp"
#include <cstring>
#include <vector>

static_assert(enc::is_encoding_v<enc::utf8>, "");
static_assert(enc::is_encoding_v<enc::utf16>, "");
static_assert(enc::is_encoding_v<enc::ascii>, "");

void ascii_util() {
    mb::ascii_string str = "Hello world!";

    char first = str[0];
    assert(first == 'H');
    assert(str.size() == std::strlen("Hello world!"));
}

void utf8_util() {
    char8_t buf[]{ 0xF0, 0x9F, 0x8C, 0x8A, 'a', 'b', 'c' };

    assert(4 == util::utf8::first_char_width(buf, buf + std::extent_v<decltype(buf)>).size);

    assert(0x1F30A == util::utf8::first_code_point(buf).codepoint);

    for(int i = 0; i < 4; i++)
        assert(enc::request_result::unexpected_src_end == util::utf8::first_code_point(buf, buf + i).result);

    char8_t invalid[]{ 0b10000000 };
    assert(enc::request_result::invalid_input == util::utf8::first_code_point(invalid).result);
}

void utf16_util() {
    char16_t buf[]{ 0xD83D, 0xDE02, 'a', 'b', 'c' };

    assert(2 == util::utf16::first_char_width(buf, buf + std::extent_v<decltype(buf)>).size);
    assert(0x1F602 == util::utf16::first_code_point(buf, buf + 2).codepoint);

    mb::utf16_string_view str = u"Hello";
}

void mb_string() {
    {
        mb::utf8_string s0 = u8"abcd";
        mb::utf8_string s1 = u8"bcd";

        std::vector<mb::utf8_string> utfs{s0, s1};

        assert(
            util::join<mb::utf8_string>(utfs.begin(), utfs.end(), mb::utf8_string(u8"_"))
            ==
            u8"abcd_bcd"
        );

        s1.insert(s1.begin(), u8"a");
        assert(s1 == s0);

        static_assert(std::is_same_v<bool, decltype(s0 == s1)>, "");
        static_assert(std::is_same_v<bool, decltype(s0 != s1)>, "");
        static_assert(std::is_same_v<bool, decltype(s0 >  s1)>, "");
        static_assert(std::is_same_v<bool, decltype(s0 <  s1)>, "");
        static_assert(std::is_same_v<bool, decltype(s0 >= s1)>, "");
        static_assert(std::is_same_v<bool, decltype(s0 <= s1)>, "");

        mb::utf8_string_view s1sv = s1.to_string_view();
        s0 = s1sv;

        assert(s0.size() == s1.size());
        assert(s0 == s1);
    }

    {
        mb::utf8_string u8str = u8"Привет Hello 😇!";
        mb::utf8_string_view u8str_v = u8str;

        assert(u8str.compare(u8str_v) == 0);

        auto ch0 = u8str[0];
        auto ch1 = u8str[1];

        assert(ch0 != ch1);
        assert(ch0 < ch1);
        assert(u8str[9] == u8str[10]);

        assert(u8str + u8" Исэнмэ!" == u8"Привет Hello 😇! Исэнмэ!");
        assert(u8" Исэнмэ!" + u8str == u8" Исэнмэ!Привет Hello 😇!");

        assert(u8str.size() == 6 + 1 + 5 + 1 + 1 + 1);
        assert(u8str.operator std::basic_string_view<char8_t>().size() == 6*2 + 1 + 5 + 1 + 4 + 1);

        std::u8string std = u8"std";
        assert(u8str + std == u8"Привет Hello 😇!std");
    }

    {
        mb::ascii_string empty_ascii_str = "";
        assert(empty_ascii_str.size() == 0);
    }

    {
        mb::utf16_string u16_hello_world = u"Hello world!";
        std::wstring_view converted_strv = u16_hello_world;

        assert(converted_strv.size() == u16_hello_world.size());

        assert(
            (u16_hello_world.template to_string<enc::ascii>()).size()
            ==
            std::strlen("Hello world!")*2
        );
    }

    {
        mb::utf8_string u8_hello_world = u8"Hello world!";

        assert(
            (u8_hello_world.template to_string<enc::ascii>())
            ==
            "Hello world!"
        );

        std::u8string str_from_rvalue_u8 = mb::utf8_string{u8"Hello?"}.to_string<char8_t>();
        assert(str_from_rvalue_u8.size() == 6);
    }

    {
        std::istringstream ss{"Hello"};
        mb::utf8_string str;
        ss >> str;
        assert(str == u8"Hello");
    }
}

int main() {
    utf8_util();
    utf16_util();
    mb_string();
}