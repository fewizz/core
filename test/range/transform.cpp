#include "range/transform.hpp"
#include "array.hpp"

[[maybe_unused]] auto transform_func = [](int i ) { return 0; };

static_assert(
	range::basic<range::transformed<array<int, 2>, decltype(transform_func)>>
);

int main() {}