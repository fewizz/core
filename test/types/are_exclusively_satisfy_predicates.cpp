#include "types/are_exclusively_satsify_predicates.hpp"
#include "types/are_satisfy_predicates.hpp"
#include "types/are_contain_type.hpp"
#include "types/count_of_type.hpp"

static_assert(
	types::are_exclusively_satsify_predicates<
		types::are_contain_type<int>
	>::for_types_of<int, int>
);

static_assert(
	types::are_exclusively_satsify_predicates<
		types::are_contain_type<int>,
		types::are_contain_type<float>,
		types::are_contain_type<bool>
	>::for_types_of<int, float, int, bool, bool>
);

static_assert(
	types::are_exclusively_satsify_predicates<
		types::count_of_type<int>::equals<2>
	>::for_types_of<int, int>
);

static_assert(
	! types::are_exclusively_satsify_predicates<
		types::count_of_type<int>::equals<2>
	>::for_types_of<int>
);

static_assert(
	types::are_exclusively_satsify_predicates<
		types::count_of_type<int>::less_or_equals<2>
	>::for_types_of<int>
);

static_assert(
	! types::are_exclusively_satsify_predicates<
		types::count_of_type<int>::less_or_equals<2>
	>::for_types_of<int, int, int>
);

static_assert(
	types::are_exclusively_satsify_predicates<
		types::count_of_type<int>::equals<2>::ignore_reference,
		types::count_of_type<float>::equals<1>::ignore_reference
	>::for_types_of<int&, float&, int>
);

int main() {}