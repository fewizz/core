#include "../../include/cxx_util/utf8.hpp"
#include "../../include/cxx_util/object.hpp"
#include "../../include/cxx_util/byte_iterator.hpp"
#include "../../include/cxx_util/codec.hpp"
#include <cassert>

static_assert(u::codec<u::utf8>);

int main() {
	std::string_view str { "a" };
	std::u8string_view smile { u8"😀" };

	assert( u::utf8::decoder_type{}.convert(u::byte_iterator{ str.begin() }) == 'a' );
	assert( u::utf8::decoder_type{}.convert(u::byte_iterator{ smile.begin() }) == 0x1F600 );

	char a = 'a';

	u::utf8::encoder_type{}.convert('b', u::obj_representation_reference{ a }.begin());

	assert(a == 'b');
}