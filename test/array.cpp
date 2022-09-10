#include <array.hpp>
#include <expression_of_type.hpp>
#include <__iterator/contiguous.hpp>
#include <__range/contiguous.hpp>
#include <__range/sized.hpp>
#include <__type/is_trivially_copyable.hpp>

static_assert(
	basic_iterator<
		decltype(range_iterator(expression_of_type<array<int, 3>>))
	>
);
static_assert(
	random_access_iterator<
		decltype(range_iterator(expression_of_type<array<int, 3>>))
	>
);

static_assert(
	contiguous_iterator<
		decltype(range_iterator(expression_of_type<array<int, 3>>))
	>
);

static_assert(
	contiguous_range<
		array<int, 3>
	>
);

static_assert(
	sized_range<
		array<int, 3>
	>
);

static_assert(!trivial<array<int, 3>>);
static_assert(trivially_copyable<array<int, 3>>);

int main() {
	array a{ 0, 1, 2 };
	if(a[0] != 0) return 1;
}