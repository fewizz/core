#include "object/operations.hpp"
#include <algorithm>
#include <cassert>
#include "iterator/iterator_reference.hpp"

consteval int f() {
	std::byte minus_one_int[sizeof(int)];
	std::ranges::fill(minus_one_int, std::byte{ 0xFF });

	auto it = std::begin(minus_one_int);
	auto prev = it;

	assert( u::read_object<int>(u::iterator_reference{ it }) == -1 );

	auto dist = std::distance(prev, it);

	assert( dist == sizeof(int) );

	it = prev;
	u::write_object(0xFF, it);
	assert( u::read_object<int>(prev) == 0xFF );

	return 0;
}

int main() {
	return f();
}