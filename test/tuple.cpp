#include "tuple.hpp"
#include "is.hpp"
#include <utility>

int main() {
	int i = 0;
	
	tuple t{ 0, i, 1};

	static_assert(is::type<decltype(t)>::template same_as<tuple<int, int&, int>>);

	auto t0 = t.template move_elements_at<0, 1>();

	static_assert(is::type<decltype(t0)>::template same_as<tuple<int, int&>>);
	
	auto t1 = t0.erase_element_at<1>();

	static_assert(is::type<decltype(t1)>::template same_as<tuple<int>>);
}