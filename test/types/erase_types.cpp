#include "types/erase_types.hpp"
#include "types/are_same.hpp"

static_assert(
	types::are_same::for_types_of<
		types::erase_types<int, bool>::for_types_of<float, int, float, bool, double>,
		types::of<float, float, double>
	>
);

int main() {}