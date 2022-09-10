#include <__range/starts_with.hpp>
#include <array.hpp>

int main() {
	char ch = 'a';
	if(!__range::starts_with(array{'a', 'b', 'c' }, array{ (char)ch, 'b' })) {
		return 1;
	}
	return 0;
}