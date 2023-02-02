#include <storage.hpp>

consteval bool f() {
	storage<int> i;

	i.cast<float>();

	return true;
}

static_assert(f());