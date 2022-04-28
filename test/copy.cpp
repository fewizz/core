#pragma once

#include <core/copy.hpp>
#include <core/equals.hpp>

consteval bool f() {
	int a[]{ 1, 2, 3, 4, 5 };
	int b[]{ 0, 0, 0, 0, 0 };
	copy{ a }.to(b);
	return equals(a, b);
}

static_assert(f());