#include <__range/copy.hpp>
#include <ranges/equals.hpp>
#include <array.hpp>

consteval bool f() {
	int a[]{ 1, 2, 3, 4, 5 };
	int b[]{ 0, 0, 0, 0, 0 };
	range::copy{ a }.to(b);
	return
		ranges::equals(a, b) &&
		ranges::equals(
			range::copy( array{ 1, 2, 3 } ).to( array{ 0, 0, 0 } ),
			array{ 1, 2, 3 }
		);
}

static_assert(f());

int main() {}