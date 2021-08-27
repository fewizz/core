#include "tuple/tuple.hpp"
#include <utility>

int main() {
	int i = 0;
	u::tuple t{ 0, i, 1};

	static_assert(std::is_same_v<decltype(t), u::tuple<int, int&, int>>);

	auto t0 = t.move<0, 1>();

	static_assert(std::is_same_v<decltype(t0), u::tuple<int, int&>>);
	
}