#include "tuple.hpp"
#include "is.hpp"
#include <utility>
#include <cassert>
#include <iostream>

int main() {
	int i = 0;
	
	tuple t{ 0, i, 1};

	static_assert(is::type<decltype(t.move_element<int&>())>::template same_as<int&>);

	static_assert(is::type<decltype(t)>::template same_as<tuple<int, int&, int>>);

	auto t0 = t.template move_elements_at<0, 1>();

	static_assert(is::type<decltype(t0)>::template same_as<tuple<int, int&>>);
	
	auto t1 = t0.erase_element_at<1>();

	static_assert(is::type<decltype(t1)>::template same_as<tuple<int>>);

	tuple t2{ 0, 0.0F, 0, 0.0F, 0, 0.0F };
	auto t3 = t2.erase_elements_at<1, 3, 5>();
	static_assert(is::type<decltype(t3)>::template same_as<tuple<int, int, int>>);

	auto t4 = t3.move_elements_same_as<int>();
	static_assert(is::type<decltype(t4)>::template same_as<tuple<int, int, int>>);

	auto t5 = t3.erase_elements_same_as<int>();
	static_assert(is::type<decltype(t5)>::template same_as<tuple<>>);

	i = 3;
	tuple t6{ 0, i, 1, 0.0F};
	t6.get([&](int&) {
		--i;
	});

	assert(i == 0);
}