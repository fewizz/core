#include "meta/contain_type.hpp"

static_assert(
	contain_type<int>
	::for_types_of<
		bool, float, int
	>
);

static_assert(
	! contain_type<int>
	::for_types_of<
		bool, float, double
	>
);

int main() { }