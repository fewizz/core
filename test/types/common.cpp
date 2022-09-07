#include <types.hpp>

static_assert(
	types<int, int&, int&>::have_common &&
	same_as<types<int, int&, int&>::common, int>
);

struct a{};

static_assert(
	!types<a, int>::have_common
);

int main() {}