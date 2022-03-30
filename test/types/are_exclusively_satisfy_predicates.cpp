#include "core/meta/types/are_exclusively_satisfying_predicates.hpp"

#include "core/meta/decayed_same_as.hpp"

static_assert(
	types::are_exclusively_satisfying_predicates<
		types::count_of_decayed_same_as<int>::equals<2>,
		types::count_of_decayed_same_as<float>::equals<1>
	>::for_types<int, int, float>
);

static_assert(
	! types::are_exclusively_satisfying_predicates<
		types::count_of_decayed_same_as<int>::equals<2>,
		types::count_of_decayed_same_as<float>::equals<1>
	>::for_types<int, int, float, float>
);