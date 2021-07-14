#include <cassert>
#include "encoding/utf8.hpp"
#include "object/bytes.hpp"

int main() {
	std::string_view str { "a" };
	std::u8string_view smile { u8"😀" };

	assert( u::utf8::decoder{}(u::byte_iterator{ str.begin() }) == 'a' );
	assert( u::utf8::decoder{}(u::byte_iterator{ smile.begin() }) == 0x1F600 );

	char a = 'a';

	u::utf8::encoder{}('b', u::obj::bytes_view{ a }.begin());

	assert(a == 'b');
}