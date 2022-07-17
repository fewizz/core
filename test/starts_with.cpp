#include <core/starts_with.hpp>
#include <core/array.hpp>

int main() {
	char ch = 'a';
	if(!starts{ array{'a', 'b', 'c'} }.with(ch, 'b')) {
		return 1;
	}
	return 0;
}