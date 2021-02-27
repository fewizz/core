#include "../../include/cxx_util/encoding/ascii.hpp"
#include "../../include/cxx_util/obj_representation.hpp"
#include <cassert>

int main() {
	char ascii_a = 'a';

	auto rep = u::obj_representation{ ascii_a };

	auto cp = enc::read_codepoint<enc::ascii>(rep);

	assert(cp == 'a');
}