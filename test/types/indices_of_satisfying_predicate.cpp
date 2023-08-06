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

static_assert(type<decay<const int&>>::is_same_as<int>);

static_assert(
	types<
		types<int, bool, int&&, const int&>::indices_of_satisfying_predicate<
			is_same_as<int>.while_decayed
		>,
		indices::of<0, 2, 3>
	>::are_same
);

int main() {}