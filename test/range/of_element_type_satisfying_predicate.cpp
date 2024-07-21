#include <array.hpp>
#include <__range/of_element_type_satisfying_predicate.hpp>

static_assert(range_of_element_type_satisfying_predicate<
	array<int, 2>,
	is_same_as<int>.while_decayed
>);

static_assert(
	range_of_element_type_satisfying_predicate<
		array<int, 2>,
		is_same_as<int>.while_decayed
	>
);

int main() {}