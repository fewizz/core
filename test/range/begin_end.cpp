#include <range/begin.hpp>
#include <range/end.hpp>
#include <array.hpp>

consteval void f0() {
	int a[10];

	static_assert(range::begin(a) == a);
	static_assert(range::end(a) == a + 10);
}

consteval void f1() {
	array<float, 10> a;

	static_assert(range::begin(a) == a.begin());
	static_assert(range::end(a) == a.end());
}

int main() {}