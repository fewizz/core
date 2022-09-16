#include <c_string.hpp>
#include <array.hpp>
#include <__range/sized.hpp>

static_assert(!sized_range<c_string_of_unknown_size>);
static_assert(contiguous_range<c_string_of_unknown_size>);

consteval bool f() {
	c_string str{ "Hi!" };
	if(!str.have_elements_equal_to(array{ 'H', 'i', '!' })) throw;

	str = c_string{ "No." };

	return true;
}

static_assert(f());

int main() {

}