#include "values.hpp"
#include "is.hpp"
#include "erase_at.hpp"

using I3 = values::of<3, 6, 9>;
using BIC = values::of<(bool)0, (int)1, (char)2>;

// size
static_assert(values::of<>::size == 0);
static_assert(I3::size == 3);
static_assert(BIC::size == 3);

// empty
static_assert(values::of<>::empty);
static_assert(! I3::empty);
static_assert(! BIC::empty);

// value at index
static_assert(I3::value_at_index<0> == 3);
static_assert(I3::value_at_index<1> == 6);
static_assert(I3::value_at_index<2> == 9);

static_assert(BIC::value_at_index<0> == 0);
static_assert(BIC::value_at_index<1> == 1);
static_assert(BIC::value_at_index<2> == 2);

// values at indices
static_assert(
	is::type<
		BIC::values_at_indices<2, 1, 0>
	>::template same_as<
		values::of<(char)2, (int)1, (bool)0>
	>
);

static_assert(
	is::type<
		BIC::values_at<indices::of<2, 1, 0>>
	>::template same_as<
		values::of<(char)2, (int)1, (bool)0>
	>
);

// front
static_assert(BIC::front == bool(0));
static_assert(I3::front == 3);

// back
static_assert(BIC::back == char(2));
static_assert(I3::back == 9);

// indices of values same as
static_assert(
	is::type<
		BIC::indices_of_values_same_as<int(1)>
	>::template same_as<
		indices::of<1>
	>
);

static_assert(
	is::type<
		I3::indices_of_values_same_as<int(9)>
	>::template same_as<
		indices::of<2>
	>
);

// erase at index
static_assert(
	is::type<
		BIC::erase_at_index<0>
	>::template same_as<
		values::of<(int)1, (char)2>
	>
);

// erase at indices
static_assert(
	is::type<
		BIC::erase_at_indices<0, 2>
	>::template same_as<
		values::of<(int)1>
	>
);

// erase values
static_assert(
	is::type<
		I3::erase_values<(int)3, (int)9>
	>::template same_as<
		values::of<(int)6>
	>
);

// index of first value
static_assert(
	BIC::index_of_first_value<(char)2> == 2
);

// count of value
static_assert(
	values::of<0,1,2,2,2,1,0>::count_of_value<2> == 3
);

int main() {}