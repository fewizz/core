#include <types.hpp>
#include <type.hpp>

static_assert(
	types<int, int, float>::are_exclusively_satisfying_predicates<
		count_of_satisfying_predicate<
			is_same_as<int>.while_decayed
		> == 2,
		count_of_satisfying_predicate<
			is_same_as<float>.while_decayed
		> == 1
	>
);

static_assert(
	! types<int, int, float, float>::are_exclusively_satisfying_predicates<
		count_of_satisfying_predicate<
			is_same_as<int>.while_decayed
		> == 2,
		count_of_satisfying_predicate<
			is_same_as<int>.while_decayed
		> == 1
	>
);

int main() {}