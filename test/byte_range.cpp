#include "../include/cxx_util/byte_range.hpp"
#include <cassert>
#include <iostream>

int main() {
	char str[5] = "Test";
	std::string_view strv{"Test"};
	u::byte_range r1{ str, str + 4 };
	u::byte_range r2{ strv };

	assert(r1.size() == r2.size());
	assert(r1 == r2);
}