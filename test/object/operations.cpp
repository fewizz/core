#include "object/operations.hpp"
#include <algorithm>
#include <cassert>
#include "iterator/referencing_iterator.hpp"

int main() {
	std::byte minus_one_int[sizeof(int)];
	std::ranges::fill(minus_one_int, std::byte{ 0xFF });

	auto it = std::begin(minus_one_int);
	auto prev = it;

	assert( u::obj::read<int>(u::referencing_iterator{ it }) == -1 );

	auto dist = std::distance(prev, it);

	assert( dist == sizeof(int) );

	it = prev;
	u::obj::write(0xFF, u::referencing_iterator{ it });
	dist = std::distance(prev, it);
	assert( dist == sizeof(int) );
	assert( u::obj::read<int>(prev) == 0xFF );

	return 0;
}