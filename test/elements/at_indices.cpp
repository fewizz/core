#include "elements/at_indices.hpp"
#include <assert.h>

int main() {
	bool b = false;
	int i = 1;

	auto val = elements::at_indices<0u, 1u, 2u, 4u>::for_elements_of(b, i, 0, 1, 2, 3);
	i = 2;
	assert(val.at_index<1u>() == 2);
}