#include "elements/pass_at_indices.hpp"
#include "../assert.hpp"

int main() {
	int b = false;

	elements::pass_at_indices<3, 2, 0>::for_function_and_elements_of([&](auto... val) {
		b = (val + ...);
	}, 1, 2, 3, 4);

	assert(b == 8);

}