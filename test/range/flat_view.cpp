#include <range.hpp>
#include <array.hpp>
#include <c_string.hpp>

consteval bool f() {
	{
		auto fv = array {
			array{ 0, 1, 2 }, array{ 3, 4, 5 }, array{ 6, 7, 8 }
		}.flat_view();
		auto it = fv.iterator();

		for(int x = 0; x < 9; ++x) {
			if(*it != x) throw;
			++it;
		}
	}

	{
		auto fv1  = array {
			c_string{ "Hello" }, c_string{ ", " },
			c_string{ "world" }, c_string{ "!" }
		}.flat_view();

		fv1.have_elements_equal_to(c_string{ "Hello, world!" });
		if(*fv1.iterator() != 'H') throw;
	}

	return true;
}

static_assert(f());

int main() {}