#include "params.hpp"
#include <iostream>

int main() {
	u::params<int, float, double> p{ 0, 0.1F, 0.2 };

	p.handle<u::optional<int>>([](auto v) {
		std::cout << v << std::endl;
	})
	.handle<u::optional<float>>([](auto v) {
		std::cout << v << std::endl;
	})
	.handle<u::optional<long>>([](auto){});
}