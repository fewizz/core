#include <array.hpp>
#include <__range/split_view.hpp>

int main() {
	constexpr array a{ 0, 3, 3, 1, 2, 1, 1, 0, 1, 1, 1 };

	static_assert(__range::split_view(a, 1)[0].size() == 3);
	static_assert(__range::split_view(a, 1)[0][0] == 0);
	static_assert(__range::split_view(a, 1)[0][1] == 3);
	static_assert(__range::split_view(a, 1)[0][2] == 3);

	static_assert(__range::split_view(a, 1)[1].size() == 1);
	static_assert(__range::split_view(a, 1)[1][0] == 2);

	static_assert(__range::split_view(a, 1)[2].size() == 1);
	static_assert(__range::split_view(a, 1)[2][0] == 0);

	static_assert(__range::split_view(a, 1, 0)[0].size() == 2);
	static_assert(__range::split_view(a, 1, 0)[0][0] == 3);
	static_assert(__range::split_view(a, 1, 0)[0][1] == 3);
}