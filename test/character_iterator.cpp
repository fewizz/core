#include "../include/cxx_util/character_iterator.hpp"
#include "../include/cxx_util/encoding/ascii.hpp"
#include <cassert>
#include <iostream>

static_assert(
	std::forward_iterator<u::character_iterator<enc::ascii, char*>>
);

int main() {
	std::string_view str { "Test" };

	auto it = u::make_character_iterator<enc::ascii>(str.begin(), str.end());
	auto e = u::make_character_iterator_end<enc::ascii>(str.begin(), str.end());

	assert(std::distance(it, e) == 4);
	assert((*it).size() == 1);
	assert(*it == std::string_view{"T"});

	int index = 0;
	for(auto ch : it) {
		assert(ch.size() == 1);
		assert(ch == u::obj_representation{ str[index++] });
	}

	
}