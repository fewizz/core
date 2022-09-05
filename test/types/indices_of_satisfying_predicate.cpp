#include <types.hpp>
#include <type.hpp>

static_assert(
	types<
		types<int, bool, float, int>::indices_of_satisfying_predicate<
			is_same_as<int>
		>,
		indices::of<0u, 3u>
	>::are_same
);

int main() {}