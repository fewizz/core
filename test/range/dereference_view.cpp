#include <__range/dereference_view.hpp>
#include <array.hpp>

consteval bool f() {
	using namespace __range;

	int a = 0, b = 5, c = 7;

	array<int*, 3> ptrs_array{ &a, &b, &c };
	dereference_view d{ ptrs_array };

	static_assert(basic_range<decltype(d)>);
	static_assert(sized_range<decltype(d)>);

	if(d.size() != 3) throw;

	if(d[0] != 0) throw;
	if(d[1] != 5) throw;
	if(d[2] != 7) throw;

	return true;
}

int main() {
	static_assert(f());
}