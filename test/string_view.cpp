#include "../include/cxx_util/string_view.hpp"
#include "../include/cxx_util/encoding/utf8.hpp"
#include <cassert>
#include <string_view>

int main() {
	u::u8_string_view str { u8"Hello!" };

	assert( std::distance(str.raw_begin(), str.raw_end()) == 6 );
	assert( std::ranges::distance(str) == 6 );
	assert( str.front() == u8'H');

	assert( str.substr(1) == std::u8string_view{ u8"ello!" });
	assert( str.substr(5) == std::u8string_view{ u8"!" });
	assert( str.substr(1, 4) == std::u8string_view{ u8"ello" } );

	{
		u::u8_string_view part{ u8"el" };
		assert( str.find(part) == 1 );
	}

	{
		u::u8_string_view part{ u8"o!" };
		assert( str.find(part) == 4 );
	}

	{
		u::u8_string_view part{ u8"No" };
		assert( str.find(part) == u::u8_string_view::npos );
	}

	
}