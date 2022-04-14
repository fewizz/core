#include <core/meta/type/is_same_as.hpp>
#include <core/integer.hpp>

static_assert(
	!type::is_same_as<uint32>::for_type<bool>
);

static_assert(
	type::is_same_as<char>::for_type<char>
);

int main() {}