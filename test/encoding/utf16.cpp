#include "../../include/cxx_util/encoding/utf16.hpp"
#include "../../include/cxx_util/obj_representation.hpp"
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
	u::obj_representation a_rep{ a };

	u::byte_iterator beg{ a_rep.begin() };
	u::byte_iterator end{ a_rep.end() };

	enc::utf16::write(enc::codepoint<enc::unicode>{'b'}, beg, end);

	assert(a == 'b');
}