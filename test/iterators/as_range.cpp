#include <iterator_and_sentinel.hpp>
#include <array.hpp>

consteval int f() {
	array a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto s = iterator_and_sentinel {
		a.iterator() + 1,
		a.sentinel() - 1
	}.as_range();
	return *(s.iterator());
}

static_assert(f() == 1);

int main() {}