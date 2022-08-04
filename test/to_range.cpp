#include <core/to_range.hpp>
#include <core/array.hpp>

consteval int f() {
	array a { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	auto s = to_range(a.begin() + 1, a.end() - 1);
	return *(s.begin());
}

static_assert(f() == 1);

int main() {}