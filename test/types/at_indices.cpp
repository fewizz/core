#include "types/at_indices.hpp"
#include "types/are_same.hpp"

static_assert(
	types::are_same::for_types_of<
		types::at_indices<1u, 4u>::template for_types_of<char, int, double, long, bool>,
		types::of<int, bool>
	>
);

int main() {}