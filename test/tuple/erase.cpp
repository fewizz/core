#include "tuple/erase.hpp"

#include <cassert>

int main() {
	std::tuple t{ 0, 1, 2, 0.5, " ", 3 };
	auto e{ u::erase<int>(t) };
	static_assert(e.size() == 4);
	assert(e.value() == 0);
}