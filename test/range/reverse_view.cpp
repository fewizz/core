#include <__range/reverse_view.hpp>
#include <__range/element_type.hpp>
#include <array.hpp>

consteval bool f() {
	using namespace __range;

	int a = 0, b = 5, c = 7;

	array<const int, 3> ptrs_array{ a, b, c };
	reverse_view d{ ptrs_array };

	static_assert(basic_range<decltype(d)>);
	static_assert(same_as<range_element_type<decltype(d)>, const int&>);

	if (d.size() != 3) throw;

	if (d[0] != 7) throw;
	if (d[1] != 5) throw;
	if (d[2] != 0) throw;

	return true;
}

int main() {
	static_assert(f());
}