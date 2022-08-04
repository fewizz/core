#include <types/are_same.hpp>
#include <values/permutations.hpp>

static_assert(
	types_are_same<
		values::permutations::for_values<0, 1>,
		types::of<
			values::of<0, 1>,
			values::of<1, 0>
		>
	>
);

static_assert(
	types_are_same<
		values::permutations::for_values<'a', 'b', 'c'>,
		types::of<
			values::of<'a', 'b', 'c'>,
			values::of<'a', 'c', 'b'>,
			values::of<'b', 'a', 'c'>,
			values::of<'b', 'c', 'a'>,
			values::of<'c', 'a', 'b'>,
			values::of<'c', 'b', 'a'>
		>
	>
);

int main() {}