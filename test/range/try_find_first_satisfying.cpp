#include <__range/try_find_first_satisfying.hpp>
#include <array.hpp>

int main() {
	{
		int a = 54;
		int b = 45;
		int c = 4;

		array<int*, 3> arr_ptrs{ &a, &b, &c };

		optional<int*&> ptr = __range::try_find_first_satisfying(
			arr_ptrs,
			[](int* ptr) { return *ptr == 45; }
		);

		if(!ptr.has_value() || *(ptr.get()) != 45) {
			return 1;
		}
	}
	{
		struct s {
			int i;
			s(int i) : i{ i } {}
			s(const s&) = delete;
			s(s&&) = delete;
		};

		array<s, 5> a{ 5, 6, 7, 8, 9 };
		optional<s&> o = __range::try_find_first_satisfying(
			a,
			[](s& s) { return s.i == 7; }
		);

		if(!o.has_value() || o.get().i != 7) {
			return 2;
		}
	}
}