#include "parameter_pack/for_each.hpp"

#include <cassert>

int main() {
	bool _int = false, _float = false, _str = false;
	u::for_each(
		0, 1.0, "asdasd",
		u::do_one_of {
			[&](int) { _int = true; },
			[&](double) { _float = true; },
			[&](const char*) { _str = true; }
		}
	);

	assert(_int && _float && _str);
}