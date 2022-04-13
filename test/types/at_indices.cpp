#include "core/meta/types/at_indices.hpp"
#include "core/meta/types/are_same.hpp"

static_assert(
	types::are_same::for_types<
		types::at_indices<1, 4>::template for_types<char, int, double, long, bool>,
		types::of<int, bool>
	>
);

int main() {}