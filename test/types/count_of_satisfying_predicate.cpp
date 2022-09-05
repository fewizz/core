#include <types.hpp>
#include <type.hpp>

static_assert(
	types<int, float, int, int, bool>::count_of_satisfying_predicate<
		is_same_as<int>
	> == 3
);

int main() {}