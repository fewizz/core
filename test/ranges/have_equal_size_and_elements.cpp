#include <__ranges/have_equal_size_and_elements.hpp>
#include <c_string.hpp>

static_assert(__ranges::have_equal_size_and_elements(
	c_string{ "Hello!" },
	c_string{ "Hello!" }
));

static_assert(__ranges::have_equal_size_and_elements(
	c_string<c_string_type::unknown_size>{ "Hello!" }.sized(),
	c_string<c_string_type::unknown_size>{ "Hello!" }.sized()
));

int main() {}