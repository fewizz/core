#pragma once

#include <core/zip.hpp>
#include <core/array.hpp>
#include <core/c_string.hpp>

consteval auto f() {
	return zip_view {
		array{ 1, 4, 9, 16 },
		array{ 'c', 'a', 's', 'j' },
		array{ 0.1, 1.0, 10.0, 1000.0 },
		c_string{ "hell" }
	};
}

static_assert(f()[0] == elements::of{ 1,  'c', 0.1,    'h' });
static_assert(f()[1] == elements::of{ 4,  'a', 1.0,    'e' });
static_assert(f()[2] == elements::of{ 9,  's', 10.0,   'l' });
static_assert(f()[3] == elements::of{ 16, 'j', 1000.0, 'l' });

static_assert(f().begin() + 4 == f().end());