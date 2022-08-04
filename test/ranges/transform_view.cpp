#include <ranges/transform_view.hpp>
#include <array.hpp>

constexpr array a{ 0, 1, 2, 4 };
constexpr array b{ 3, 6, 3, 2 };

auto constexpr f() {
	return ranges::transform_view {
		a, b,
		[](auto a, auto b) {
			return a + b;
		}
	};
}

static_assert(f()[0] == 3);
static_assert(f()[1] == 7);
static_assert(f()[2] == 5);
static_assert(f()[3] == 6);

static_assert(f().begin() + 4 == f().end());
static_assert(f().begin() + 3 != f().end());

int main() {}