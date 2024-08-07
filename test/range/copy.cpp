#include <range.hpp>
#include <array.hpp>

consteval bool f() {
	array a{ 1, 2, 3, 4, 5 };
	array b{ 0, 0, 0, 0, 0 };
	a.copy_to(b);

	array<int, 3> copy_to;
	array{ 1, 2, 3 }.copy_to(copy_to);

	if (!a.has_equal_size_and_elements(b)) throw;
	if (!copy_to.has_equal_size_and_elements(array{ 1, 2, 3 })) throw;
	return true;
}

static_assert(f());

int main() {}