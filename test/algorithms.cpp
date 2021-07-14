#include "algorithms.hpp"

#include <vector>
#include <array>
#include <cassert>

int main() {
	std::vector v0{ 0, 1, 2, 4, 6, 7, 9 };
	std::array<int, 3> v1{ 6, 7, 9 };

	assert(u::ends_with(v0, v0));
	assert(u::ends_with(v0, v1));

	std::vector v2{ 6, 7 };
	assert(!u::ends_with(v0, v2));
}