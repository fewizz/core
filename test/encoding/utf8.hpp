#include "../../include/cxx_util/encoding/utf8.hpp"

#include <cassert>

consteval int consteval_test_utf8() {
	int arr[] { 'a' };

	u::byte_iterator beg{ arr };
	u::byte_iterator end{ arr + 1 };

	enc::utf8::write(enc::codepoint<enc::unicode>{'b'}, beg, end);

	assert(arr[0] == 'b');

	return 0;
}

inline void test_utf8() {
	using namespace enc;
	
	constexpr std::string_view str { "a" };
	constexpr std::u8string_view smile { u8"😀" };

	static_assert( size<utf8>(str) == 1 );
	static_assert( size<utf8>(smile) == 4 );

	static_assert( read_codepoint<utf8>(str) == 'a' );
	static_assert( read_codepoint<utf8>(smile) == 0x1F600 );

	constexpr int i = consteval_test_utf8();
}