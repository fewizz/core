#include <__types/are_same.hpp>
#include <__values/permutations.hpp>

static_assert(
	types_are_same<
		__values::permutations::for_values<0, 1>,
		__types::of<
			__values::of<0, 1>,
			__values::of<1, 0>
		>
	>
);

static_assert(
	types_are_same<
		__values::permutations::for_values<'a', 'b', 'c'>,
		__types::of<
			__values::of<'a', 'b', 'c'>,
			__values::of<'a', 'c', 'b'>,
			__values::of<'b', 'a', 'c'>,
			__values::of<'b', 'c', 'a'>,
			__values::of<'c', 'a', 'b'>,
			__values::of<'c', 'b', 'a'>
		>
	>
);

int main() {}