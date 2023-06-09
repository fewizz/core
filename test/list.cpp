#include <list.hpp>
#include <array.hpp>

static_assert(contiguous_iterator<storage<int>*>);
static_assert(contiguous_iterator<storage_of_size_and_alignment<1, 1>*>);

int main() {
	{
		array<storage<int>, 5> storage_range{};
		list l{ storage_range };

		if(l.size() != 0) return 1;

		l.emplace_back<int>(42);

		if(l.size() != 1) return 2;
		if(*l.iterator() != 42) return 3;
		if(l[0] != 42) return 4;
		if(!l.contains(42)) return 5;
	}
	{
		int a = 0, b = 1;
		array<storage<int*>, 2> storage_range;

		list l{ storage_range };
		if(l.size() != 0) return 1;
		l.emplace_back(&a);
		l.emplace_back(&b);

		*l[0] = 42;

		if(a != 42) return 1;
	}
}