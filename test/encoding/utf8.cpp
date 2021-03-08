#include "../../include/cxx_util/encoding/utf8.hpp"
#include "../../include/cxx_util/obj_representation.hpp"
#include <cassert>

static_assert(enc::char_set<enc::unicode>);

int main() {
	using namespace enc;
	
	std::string_view str { "a" };
	std::u8string_view smile { u8"😀" };

	assert( size<utf8>(str) == 1 );
	assert( size<utf8>(smile) == 4 );

	assert( read_codepoint<utf8>(str) == 'a' );
	assert( read_codepoint<utf8>(smile) == 0x1F600 );

	char a = 'a';

	write_codepoint<utf8>(
		enc::codepoint<enc::unicode>{'b'},
		u::obj_representation{ a }
	);

	assert(a == 'b');
}