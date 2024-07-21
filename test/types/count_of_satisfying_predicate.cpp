#include <types.hpp>
#include <type.hpp>

static_assert(
	(count_of<
		is_same_as<int>
	> == 3).for_types<int, float, int, int, bool>()
);

static_assert(
	types<int, float, int, int, bool>::count_of<
		is_same_as<int>
	> == 3
);

int main() {}