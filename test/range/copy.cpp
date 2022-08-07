#include <__range/copy.hpp>
#include <__ranges/are_equal.hpp>
#include <array.hpp>

consteval bool f() {
	int a[]{ 1, 2, 3, 4, 5 };
	int b[]{ 0, 0, 0, 0, 0 };
	__range::copy{ a }.to(b);

	array<int, 3> copy_to;
	__range::copy( array{ 1, 2, 3 } ).to(copy_to);

	if(!__ranges::are_equal(a, b)) throw;
	if(!__ranges::are_equal(copy_to, array{ 1, 2, 3 })) throw;
	return true;
}

static_assert(f());

int main() {}