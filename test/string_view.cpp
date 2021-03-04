#include "../include/cxx_util/string_view.hpp"
#include "../include/cxx_util/encoding/utf8.hpp"
#include <cassert>
#include <string_view>

int main() {
	u::basic_string_view<enc::utf8, const char8_t*> str { u8"Hello!" };

	assert( std::distance(str.raw_begin(), str.raw_end()) == 6 );
	assert( std::ranges::distance(str) == 6 );
	assert( str.front() == std::u8string_view{ u8"H" });
	assert( str.substr(1) == std::u8string_view{ u8"ello!" });
}