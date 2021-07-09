#include "iterator/referencing_iterator.hpp"
#include "iterator/iterator.hpp"
#include <algorithm>
#include <array>
#include <cassert>

static_assert( std::input_or_output_iterator<u::referencing_iterator<int*>> );
static_assert( std::input_iterator<u::referencing_iterator<int*>> );
static_assert( std::output_iterator<u::referencing_iterator<int*>, int> );

int main() {
	std::array<int, 42> arr;
	std::ranges::fill(arr, 0);

	auto it = arr.begin();
	auto ref = u::referencing_iterator{ it };

	++ref;
	assert(it == arr.begin() + 1);

	return 0;
}