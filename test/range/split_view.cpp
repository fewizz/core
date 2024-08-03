#include <array.hpp>
#include <__range/split_view.hpp>
#include <c_string.hpp>


consteval bool f() {
	array a{ 0, 3, 3, 1, 2, 1, 1, 0, 1, 1, 1 };

	if (__range::split_view(a, 1)[0].size() != 3) throw;
	if (__range::split_view(a, 1)[0][0] != 0) throw;
	if (__range::split_view(a, 1)[0][1] != 3) throw;
	if (__range::split_view(a, 1)[0][2] != 3) throw;

	if (__range::split_view(a, 1)[1].size() != 1) throw;
	if (__range::split_view(a, 1)[1][0] != 2) throw;

	if (__range::split_view(a, 1)[2].size() != 1) throw;
	if (__range::split_view(a, 1)[2][0] != 0) throw;

	if (__range::split_view(a, 1, 0)[0].size() != 2) throw;
	if (__range::split_view(a, 1, 0)[0][0] != 3) throw;
	if (__range::split_view(a, 1, 0)[0][1] != 3) throw;

	{
		int i = 0;
		for (auto _ : "0"sv.split_view(',')) {
			++i;
		}
		if (i != 1) throw;
	}

	{
		span s{"735,346.820809248555,4,2,1,55,1,0"sv};

		if (__range::split_view(s, ',')[0].size() != 3) throw;
		if (__range::split_view(s, ',')[1].size() != 16) throw;
		if (__range::split_view(s, ',')[2].size() != 1) throw;
		if (__range::split_view(s, ',')[3].size() != 1) throw;
		if (__range::split_view(s, ',')[4].size() != 1) throw;
		if (__range::split_view(s, ',')[5].size() != 2) throw;
		if (__range::split_view(s, ',')[6].size() != 1) throw;
		if (__range::split_view(s, ',')[7].size() != 1) throw;

		int i = 0;
		for (auto _ : __range::split_view(s, ',')) {
			++i;
		}
		if (i != 8) throw;
	}

	return true;
}

static_assert(f());

int main() {
}