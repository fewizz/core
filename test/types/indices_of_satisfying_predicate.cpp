#include "type/is_same_as.hpp"
#include "types/indices_of_satisfying_predicate.hpp"

static_assert(
	types::are_same::for_types_of<
		types::indices_of_satisfying_predicate<
			type::is_same_as<int>
		>::template for_types_of<int, bool, float, int>,
		indices_of<0u, 3u>
	>
);

int main() {}