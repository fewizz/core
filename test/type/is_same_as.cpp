#include <type/is_same_as.hpp>
#include <integer.hpp>

static_assert(
	!type::is_same_as<uint32>::for_type<bool>
);

static_assert(
	type::is_same_as<char>::for_type<char>
);

int main() {}