#include <types.hpp>
#include <type.hpp>

static_assert(
	types<bool, bool, int>::are_satisfying_predicates<
		count_of_satisfying_predicate<is_same_as<bool>> == 2,
		count_of_satisfying_predicate<is_same_as<int>>  == 1
	>
);

int main() {}