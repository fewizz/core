#include "../../include/cxx_util/encoding/ascii.hpp"
#include "../../include/cxx_util/obj_representation.hpp"
#include <cassert>

static_assert(enc::char_set<enc::ascii>);
static_assert(enc::encoding<enc::ascii>);

int main() {
	char ascii_a = 'a';

	auto rep = u::obj_representation{ ascii_a };

	auto cp = enc::read_codepoint<enc::ascii>(rep);

	assert(cp == 'a');
}