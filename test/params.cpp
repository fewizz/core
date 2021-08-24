#include "params.hpp"
#include <iostream>

int main() {
	int i = 1;
	u::params p{ 0, 0.1F, 0.2, i };

	/*u::params p1{
		std::tuple<int&>(i)
	};*/

	static_assert(
		std::is_same_v<
			std::tuple_element_t<0, std::remove_reference_t<decltype(p.tuple())>>,
			int
		>
	);

	static_assert(
		std::is_same_v<
			std::tuple_element_t<3, std::remove_reference_t<decltype(p.tuple())>>,
			int &
		>
	);

	p.handle<u::optional<int>>([](auto v) {
		std::cout << v << std::endl;
	})
	.handle<u::optional<float>>([](auto v) {
		std::cout << v << std::endl;
	})
	.handle<u::optional<long>>([](auto){})
	.handle<u::optional<double>>([](auto){})
	.handle<u::optional<int&>>([](auto){})
	.check_for_emptiness();
}