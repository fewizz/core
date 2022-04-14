#include "core/meta/elements/of.hpp"
#include "core/move.hpp"

struct a {
	constexpr a() = default;
	constexpr a(a&&) = default;
	constexpr a(const a&) = delete;
};

consteval elements::of<int, float, bool> some_elements() {
	return { 1, 0.0F, false };
}

consteval int f() {
	elements::of elems{ 0, 1.0F, a{} };
	[[maybe_unused]] auto elems1 = move(elems);

	[[maybe_unused]] auto [ i, f, b ] = some_elements();
	return 0;
}

int main() {
	static_assert(f() == 0);
}