#include "elements/of.hpp"
#include "move.hpp"

struct a {
	constexpr a() = default;
	constexpr a(a&&) = default;
	constexpr a(const a&) = delete;
};

consteval void f() {
	elements::of elems{ 0, 1.0F, a{} };
	auto elems1 = move(elems);
}

int main() {
	f();
}