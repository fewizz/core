#include <list.hpp>
#include <array.hpp>

static_assert(contiguous_iterator<storage<char>*>);
static_assert(base_of<
	storage_range_element_iterator<storage<char>*>,
	contiguous_iterator_mark
>);
static_assert(contiguous_iterator<
	storage_range_element_iterator<storage<char>*>
>);

int main() {
	{
		array<storage<int>, 5> storage_range{};
		list l{ storage_range };

		if(l.size() != 0) return 1;

		l.emplace_back(42);

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