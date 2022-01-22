#include "meta/types/are_exclusively_satsify_predicates.hpp"

#include "meta/types/count_of_decayed_same_as.hpp"

static_assert(
	types::are_exclusively_satsify_predicates<
		types::count_of_decayed_same_as<int>::equals<2>,
		types::count_of_decayed_same_as<float>::equals<1>
	>::for_types<int, int, float>
);

static_assert(
	! types::are_exclusively_satsify_predicates<
		types::count_of_decayed_same_as<int>::equals<2>,
		types::count_of_decayed_same_as<float>::equals<1>
	>::for_types<int, int, float, float>
);