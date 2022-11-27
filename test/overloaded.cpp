#include <overloaded.hpp>

consteval bool f() {
	auto f = overloaded {
		[](float){ return 0.0; },
		[](int){}
	}.then([](float i = 1.0) {
		return i;
	});

	if(f(0) != 1.0) throw;
	if(f(0.0f) != 0.0) throw;

	return true;
}

static_assert(f());

int main() {}