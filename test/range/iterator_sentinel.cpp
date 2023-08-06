#include <__range/iterator.hpp>
#include <__range/sentinel.hpp>
#include <array.hpp>

consteval void f0() {
	int a[10];

	static_assert(range_iterator(a) == a);
	static_assert(range_sentinel(a) == a + 10);
}

consteval void f1() {
	array<float, 10> a;

	static_assert(range_iterator(a) == a.iterator());
	static_assert(range_sentinel(a) == a.sentinel());
}

int main() {}