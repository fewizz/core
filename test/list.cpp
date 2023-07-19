#include <list.hpp>
#include <array.hpp>

static_assert(contiguous_iterator<storage<int>*>);
static_assert(contiguous_iterator<storage_of_size_and_alignment<1, 1>*>);

int main() {
	{
		array<storage<int>, 5> storage_range{};
		list l{ storage_range };

		if(l.size() != 0) throw;

		l.emplace_back<int>(42);

		if(l.size() != 1) throw;
		if(*l.iterator() != 42) throw;
		if(l[0] != 42) throw;
		if(!l.contains(42)) throw;
	}
	{
		int a = 0, b = 1;
		array<storage<int*>, 2> storage_range;

		list l{ storage_range };
		if(l.size() != 0) throw;
		l.emplace_back(&a);
		l.emplace_back(&b);

		*l[0] = 42;

		if(a != 42) throw;
	}
}