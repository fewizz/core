#include "../include/cxx_util/math.hpp"
#include <cassert>

int main() {
	u::div_t<int> res{0,0};

	res = u::div_floor(1, 3);
	assert(res.quot == 0 && res.rem == 1);

	res = u::div_floor(4, 3);
	assert(res.quot == 1 && res.rem == 1);

	res = u::div_floor(-1, 3);
	assert(res.quot == -1 && res.rem == 2);

	res = u::div_floor(-4, 3);
	assert(res.quot == -2 && res.rem == 2);
}