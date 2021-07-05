#include "bit.hpp"
#include <cassert>

int main() {
	int val = 42;

	u::bit_reference b{ val, 0 };

	assert(b.get() == 0);

	b.set(1);

	assert(val == 43);

	

	for()
}