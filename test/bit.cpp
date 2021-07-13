#include "bit.hpp"
#include <cassert>
#include <iterator>
#include <algorithm>

static_assert(std::input_iterator<u::bits_iterator<int>>);
static_assert(std::output_iterator<u::bits_iterator<int>, bool>);
static_assert(std::forward_iterator<u::bits_iterator<int>>);
static_assert(std::bidirectional_iterator<u::bits_iterator<int>>);
static_assert(std::random_access_iterator<u::bits_iterator<int>>);

static_assert(std::ranges::range<u::bits_reference<int>>);
static_assert(std::ranges::sized_range<u::bits_reference<int>>);
static_assert(std::ranges::input_range<u::bits_reference<int>>);
static_assert(std::ranges::output_range<u::bits_reference<int>, bool>);
static_assert(std::ranges::random_access_range<u::bits_reference<int>>);

int main() {
	int val = 42;

	u::bit_reference b{ val, 0 };

	assert(b == 0);

	b = 1;

	assert(val == 43);

	u::bits_reference ref{ val };
	static_assert(ref.size() == 8*sizeof(val));

	std::array<bool, 6> possible{1, 1, 0, 1, 0, 1};

	for(auto i = 0; i < possible.size(); i++) {
		assert(possible[i] == ref[i]);
	}
}