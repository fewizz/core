#include <types.hpp>

static_assert(
	same_as<types<int, int&, int&>::common, int>
);

int main() {}