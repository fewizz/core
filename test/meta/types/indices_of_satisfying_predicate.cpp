#include <core/meta/type/is_same_as.hpp>
#include <core/meta/types/indices_of_satisfying_predicate.hpp>

static_assert(
	types::are_same::for_types<
		types::indices_of_satisfying_predicate<
			type::is_same_as<int>
		>::template for_types<int, bool, float, int>,
		indices::of<0u, 3u>
	>
);

int main() {}