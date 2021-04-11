#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include "character_iterator.hpp"
#include "ascii.hpp"

static_assert(
	std::input_iterator<
		u::encoded_string_iterator<u::ascii, std::byte*>
	>
);

int main() {
	std::string_view str { "Test" };

	u::byte_range str_bytes{ str };
	using it_t = decltype(str_bytes.begin());

	auto b = u::encoded_string_iterator<u::ascii, it_t>{ str_bytes.begin() };
	auto e = u::encoded_string_iterator<u::ascii, it_t>{ str_bytes.end() };

	assert(std::distance(b, e) == 4);

	int index = 0;
	std::for_each(b, e, [&](auto ch) {
		assert(ch == str[index++]);
	});
}