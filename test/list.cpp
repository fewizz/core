#include <list.hpp>
#include <array.hpp>

int main() {
	array<storage<int>, 5> storage_range{};
	list l{ storage_range };

	if(l.size() != 0) return 1;

	l.emplace_back(42);

	if(l.size() != 1) return 2;
	if(*l.iterator() != 42) return 3;
	if(l[0] != 42) return 4;
	if(!l.contains(42)) return 5;
}