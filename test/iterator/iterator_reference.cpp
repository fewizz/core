#include "iterator/iterator_reference.hpp"
#include "iterator/iterator.hpp"
#include <algorithm>
#include <array>
#include <cassert>

static_assert( std::input_iterator<u::iterator_reference<int*>> );
static_assert( std::random_access_iterator<u::iterator_reference<int*>> );

consteval int f() {
	std::array<int, 42> arr;
	std::ranges::fill(arr, 0);

	auto it = arr.begin();
	auto it_ref = u::iterator_reference{ it };

	assert(it_ref == it_ref);

	++it_ref;
	assert(it == arr.begin() + 1);

	assert((it_ref--).base() == arr.begin() + 1);
	assert(it == arr.begin());

	std::for_each_n(it_ref, 42, [](auto v){});

	assert(it == arr.end());

	return 0;
}

int main() {
	return f();
}