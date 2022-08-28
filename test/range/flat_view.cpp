#pragma once

#include <range.hpp>
#include <c_string.hpp>

consteval bool f() {
	{
		__range::flat_view fv { array {
			array{ 0, 1, 2 }, array{ 3, 4, 5 }, array{ 6, 7, 8 }
		}};
		auto it = fv.iterator();

		for(int x = 0; x < 9; ++x) {
			if(*it != x) throw;
			++it;
		}
	}

	{
		__range::flat_view fv1 { array {
			c_string{ "Hello" }, c_string{ ", " },
			c_string{ "world" }, c_string{ "!" }
		}};

		range{ fv1 }.equals_to(c_string{ "Hello, world!" });
	}

	return true;
}

static_assert(f());