#include <ranges/are_equal.hpp>
#include <c_string.hpp>

static_assert(ranges_are_equal(c_string{ "Hello!" }, c_string{ "Hello!" }));

static_assert(ranges_are_equal(
	c_string<c_string_type::unknown_size>{ "Hello!" },
	c_string<c_string_type::unknown_size>{ "Hello!" }
));