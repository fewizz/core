#include "object/bits.hpp"
#include <cassert>
#include <iterator>
#include <algorithm>

static_assert(std::input_iterator<u::obj::bits_iterator<int>>);
static_assert(std::output_iterator<u::obj::bits_iterator<int>, bool>);
static_assert(std::forward_iterator<u::obj::bits_iterator<int>>);
static_assert(std::bidirectional_iterator<u::obj::bits_iterator<int>>);
static_assert(std::random_access_iterator<u::obj::bits_iterator<int>>);

static_assert(std::ranges::range<u::obj::bits_view<int>>);
static_assert(std::ranges::sized_range<u::obj::bits_view<int>>);
static_assert(std::ranges::input_range<u::obj::bits_view<int>>);
static_assert(std::ranges::output_range<u::obj::bits_view<int>, bool>);
static_assert(std::ranges::random_access_range<u::obj::bits_view<int>>);

static_assert(std::ranges::range<u::obj::bits<std::byte> &>);

int main() {
	int val = 42;

	u::obj::bit_ref b{ val, 0 };

	assert(b == 0);

	b = 1;

	assert(val == 43);

	u::obj::bits_view ref{ val };
	static_assert(ref.size() == 8*sizeof(val));

	std::array<bool, 6> possible{1, 1, 0, 1, 0, 1};

	for(auto i = 0; i < possible.size(); i++) {
		assert(possible[i] == ref[i]);
	}
}