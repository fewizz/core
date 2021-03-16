#include "../../include/cxx_util/ascii.hpp"
#include "../../include/cxx_util/object.hpp"
#include "../../include/cxx_util/codec.hpp"
#include <cassert>

static_assert(u::character_set<u::ascii>);
static_assert(u::codec<u::ascii>);

int main() {
	char ascii_a = 'a';

	u::obj_representation_copy rep{ ascii_a };

	auto cp = u::ascii::decoder_type{}.convert(rep.begin());

	assert(cp == 'a');
}