#include <c_string.hpp>
#include <array.hpp>
#include <__range/sized.hpp>

static_assert(!sized_range<c_string_of_unknown_size<char>>);
static_assert(contiguous_range<c_string_of_unknown_size<char>>);

consteval bool f() {
	c_string str{ "Hi!" };

	if(str.size() != 3) throw;
	if(!str.has_equal_size_and_elements(array{ 'H', 'i', '!' })) throw;

	str = c_string{ "No." };

	if(str.size() != 3) throw;
	if(!str.has_equal_size_and_elements(array{ 'N', 'o', '.' })) throw;

	struct str_t : c_string_of_known_size<char> {
		using c_string_of_known_size<char>::c_string;
	};

	if(str_t{ "asd" }.size() != 3) throw;

	return true;
}

static_assert(f());

int main() {

}