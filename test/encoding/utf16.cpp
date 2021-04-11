#include "utf16.hpp"
#include "obj_representation.hpp"
#include <cassert>

int main() {
	using namespace enc;
	
	std::u16string_view str { u"a" };
	std::u16string_view smile { u"😀" };

	assert( size<utf16>(str) == 1 );
	assert( size<utf16>(smile) == 2 );

	assert( read_codepoint<utf16>(str) == 'a' );
	assert( read_codepoint<utf16>(smile) == 0x1F600 );

	char16_t a = 'a';

	write_codepoint<utf16>(
		codepoint<enc::unicode>{'b'},
		u::obj_representation{ a }
	);

	assert(a == 'b');
}