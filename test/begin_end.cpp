#include <core/begin.hpp>
#include <core/end.hpp>
#include <core/array.hpp>

consteval void f0() {
	int a[10];

	static_assert(begin(a) == a);
	static_assert(end(a) == a + 10);
}

consteval void f1() {
	array<float, 10> a;

	static_assert(begin(a) == a.begin());
	static_assert(end(a) == a.end());
}

int main() {}