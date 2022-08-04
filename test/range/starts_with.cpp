#include <range/starts_with.hpp>
#include <array.hpp>

int main() {
	char ch = 'a';
	if(!range::starts{ array{'a', 'b', 'c'} }.with(ch, 'b')) {
		return 1;
	}
	return 0;
}