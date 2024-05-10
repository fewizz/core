#include <__ranges/have_equal_size_and_elements.hpp>
#include <ranges.hpp>
#include <c_string.hpp>

static_assert(__ranges::have_equal_size_and_elements(
	c_string{ "Hello!" },
	c_string{ "Hello!" }
));

static_assert(__ranges::have_equal_size_and_elements(
	c_string{ "Hello!" }.sized(),
	c_string{ "Hello!" }.sized()
));

int main() {}