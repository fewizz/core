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

consteval void f() {
	elements::of elems{ 0, 1.0F, a{} };
	auto elems1 = move(elems);

	auto [ i, f, b ] = some_elements();
}

int main() {
	f();
}