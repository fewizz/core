#include "../include/cxx_util/string.hpp"
#include <cassert>
#include <iostream>

int main() {
	{
		const char c_str[] = "ASCII";
		u::ascii_string str { c_str };
		assert(std::ranges::distance(str) == 6);
		assert(str.front() == 'A');
	}

	u::u8_string str { u8"Hello!"};
	assert(std::ranges::distance(str) == 6);
}