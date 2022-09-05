#include <types.hpp>

static_assert(
	types<
		types<char, int, double, long, bool>::at_indices<1, 4>,
		types<int, bool>
	>::are_same
);

int main() {}