#include "params.hpp"
#include <array>

struct A {
	A() = default;
	A(A&&) = default;
	A(const A&) = delete;
};
struct B {
	B() = default;
	B(B&&) = default;
	B(const B&) = delete;
};

int main() {
	int i = 1;
	float f = 0;
	double d = 1;
	std::array<int, 3> a{ 0, 1, 2 };
	u::params p{ A{}, i, f, d, a, B{} };

	p.handle<u::required>([](int& v) {})
	.handle<u::required>([](float& v) {})
	.handle<u::optional>([](long&){})
	.handle<u::required>([](double&){})
	.handle<u::required>([]<std::size_t S>(std::array<int, S>&){})
	.handle<u::required>([](A){})
	.handle<u::required>([](B){})
	.check_for_emptiness();
}