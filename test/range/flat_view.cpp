#include <range.hpp>
#include <array.hpp>
#include <c_string.hpp>

constexpr bool f() {
	{
		auto fv = array {
			array{ 0, 1, 2 }, array{ 3, 4, 5 }, array{ 6, 7, 8 }
		}.flat_view();

		static_assert(!decltype(fv)::is_borrowed_range);

		auto it = fv.iterator();

		for (int x = 0; x < 9; ++x) {
			if (*it != x) throw;
			++it;
		}
	}

	{
		array a {
			array{ 0, 1, 2 }, array{ 3, 4, 5 }, array{ 6, 7, 8 }
		};

		static_assert(decltype(a.flat_view())::is_borrowed_range);

		auto tv = a.transform_view([](auto& arr) { return array{ arr[0] }; });
		static_assert(same_as<
			range_element_type<decltype(tv)>,
			array<int, 1>
		>);
		static_assert(borrowed_range<decltype(tv)>);
	}

	{
		auto fv  = array {
			c_string{ "Hello" }, c_string{ ", " },
			c_string{ "world" }, c_string{ "!" }
		}.flat_view();

		fv.has_equal_size_and_elements(c_string{ "Hello, world!" });
		if (*fv.iterator() != 'H') throw;
	}

	return true;
}

int main() {
	static_assert(f());
	f();
}