#include <c_string.hpp>
#include <array.hpp>
#include <__range/sized.hpp>

static_assert(!sized_range<c_string<char>>);
static_assert(contiguous_range<c_string<char>>);

consteval bool f() {
	c_string str{ +"Hi!" };

	if(str.get_or_compute_size() != 3) throw;
	if(!str.has_equal_size_and_elements(array{ 'H', 'i', '!' })) throw;

	str = c_string{ "No." };

	if(str.get_or_compute_size() != 3) throw;
	if(!str.has_equal_size_and_elements(array{ 'N', 'o', '.' })) throw;

	return true;
}

static_assert(f());

int main() {

}