#include <core/transform.hpp>
#include <core/array.hpp>

[[maybe_unused]] auto transform_func = [](int) { return 0; };

static_assert(
	range<transformed<array<int, 2>, decltype(transform_func)>>
);

int main() {}