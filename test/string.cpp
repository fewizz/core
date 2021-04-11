#include "string.hpp"
#include <cassert>
#include <iostream>

int main() {
	{
		const char c_str[] = "ASCII";
		u::ascii_string str { c_str };
		assert(std::ranges::distance(str) == 6);
		assert(str.front() == 'A');
		auto second_ch = *(str.begin() + 1);
		std::cout << second_ch.codepoint().value() << std::endl;
		assert(second_ch == enc::codepoint<enc::unicode>('S'));
	}

	u::u8_string str { u8"Hello!"};
	assert(std::ranges::distance(str) == 6);
}