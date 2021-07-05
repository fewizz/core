#include "object/object_representation.hpp"
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <iostream>

consteval int f() {
	bool big = std::endian::native == std::endian::big;

	auto check_front_and_back = [&](auto& c, int low) {
		assert(c.front() == std::byte( big ? 0 : low ) );
		assert(c.back() == std::byte( big ? low : 0 ) );
	};

	int num = 0x42;

	// create by reference
	u::object_representation_reference rep{ num };
	assert(rep.size() == sizeof(int));
	// object copy creation from representation
	assert( rep.create() == 0x42 );

	check_front_and_back(rep, 0x42);
	num = 0xFF; // changes object representation
	check_front_and_back(rep, 0xFF);

	auto rep_copy = u::object_representation_copy{ num };
	check_front_and_back(rep_copy, 0xFF);
	num = 0x01; // should not change copied object representation
	check_front_and_back(rep_copy, 0xFF);

	assert( rep_copy.create() == 0xFF );

	// convert to array
	auto int_bytes = u::object_representation_copy<int>{ 0x44 };
	int_bytes[big ? sizeof(int) - 1 : 0] = std::byte{ 0x11 };

	// create from iterator
	assert(
		int_bytes.create()
		== 0x11
	);

	return 0;
}

int main() {
	return f();
}