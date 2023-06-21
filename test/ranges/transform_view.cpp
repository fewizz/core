#include <__ranges/transform_view.hpp>
#include <array.hpp>
#include <c_string.hpp>

constexpr array a{ 0, 1, 2, 4 };
constexpr array b{ 3, 6, 3, 2 };
constexpr c_string c{ "Hello" };

auto constexpr f(auto&& c) {
	return __ranges::transform_view {
		c, [](auto e) { return e * 4; }
	};
}

static_assert(f(a)[0] == 0);
static_assert(f(b)[0] == 12);
static_assert(f(a)[1] == 4);
static_assert(f(b)[1] == 24);
static_assert(f(a)[3] == 16);
static_assert(f(b)[3] == 8);
static_assert((++(++(++(++f(b).iterator())))) == f(b).sentinel());
static_assert(f(b).iterator() != f(b).sentinel());

static_assert(*(f(a).iterator() + 0) == 0);
static_assert(*(f(a).iterator() + 1) == 4);
static_assert(*(f(a).iterator() + 2) == 8);
static_assert(*(f(a).iterator() + 3) == 16);

static_assert(
	__ranges::transform_view { c, [](auto){ return 0; }}[0] == 0
);

/*auto constexpr f0() {
	return __ranges::transform_view {
		a, b,
		[](auto a, auto b) {
			return a + b;
		}
	};
}

static_assert(f0()[0] == 3);
static_assert(f0()[1] == 7);
static_assert(f0()[2] == 5);
static_assert(f0()[3] == 6);

static_assert(f0().iterator() + 4 == f().end());
static_assert(f0().iterator() + 3 != f().end());*/

int main() {}