#pragma once

#include <core/copy.hpp>
#include <core/equals.hpp>
#include <core/array.hpp>

consteval bool f() {
	int a[]{ 1, 2, 3, 4, 5 };
	int b[]{ 0, 0, 0, 0, 0 };
	copy{ a }.to(b);
	return
		equals(a, b) &&
		equals(
			copy( array{ 1, 2, 3 } ).to( array{ 0, 0, 0 } ),
			array{ 1, 2, 3 }
		);
}

static_assert(f());