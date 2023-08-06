#include <type.hpp>
#include <integer.hpp>

static_assert(
	!type<uint32>::is_same_as<bool>
);

static_assert(
	type<char>::is_same_as<char>
);

static_assert((!(is_same_as<int> && is_same_as<float>)).for_type<bool>());

int main() {}