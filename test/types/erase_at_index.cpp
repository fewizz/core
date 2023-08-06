#include <types.hpp>

static_assert(
	types<
		types<float, int>::erase_at_index<0>,
		types<int>
	>::are_same
);

static_assert(
	types<
		types<float, int>::erase_at_index<1>,
		types<float>
	>::are_same
);

static_assert(
	types<
		types<bool, bool, int, float>::erase_at_index<2>,
		types<bool, bool, float>
	>::are_same
);

int main() {}