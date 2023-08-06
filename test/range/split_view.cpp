#include <array.hpp>
#include <__range/split_view.hpp>

consteval bool f() {
	array a{ 0, 3, 3, 1, 2, 1, 1, 0, 1, 1, 1 };

	if(__range::split_view(a, 1)[0].size() != 3) throw;
	if(__range::split_view(a, 1)[0][0] != 0) throw;
	if(__range::split_view(a, 1)[0][1] != 3) throw;
	if(__range::split_view(a, 1)[0][2] != 3) throw;

	if(__range::split_view(a, 1)[1].size() != 1) throw;
	if(__range::split_view(a, 1)[1][0] != 2) throw;

	if(__range::split_view(a, 1)[2].size() != 1) throw;
	if(__range::split_view(a, 1)[2][0] != 0) throw;

	if(__range::split_view(a, 1, 0)[0].size() != 2) throw;
	if(__range::split_view(a, 1, 0)[0][0] != 3) throw;
	if(__range::split_view(a, 1, 0)[0][1] != 3) throw;

	return true;
}

static_assert(f());

int main() {
}