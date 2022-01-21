#include "types/erase_at_index.hpp"

static_assert(
	types::are_same::for_types<
		types::erase_at_index<0>::for_types<float, int>,
		types::of<int>
	>
);

static_assert(
	types::are_same::for_types<
		types::erase_at_index<1>::for_types<float, int>,
		types::of<float>
	>
);

int main() {}