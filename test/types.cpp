#include "types.hpp"
#include "is.hpp"

using IFB = types::of<int, float, bool>;
using I3 = types::of<int, int, int>;

// empty
static_assert(types::of<>::empty);
static_assert(!IFB::empty);

// at index
static_assert(is::type<IFB::at_index<0>>::template same_as<int>);

// at indices
static_assert(
	is::type<
		IFB::at_indices<0, 1>
	>::template same_as<
		types::of<int, float>
	>
);

// front
static_assert(is::type<IFB::front>::template same_as<int>);

// back
static_assert(is::type<IFB::back>::template same_as<bool>);

// indices of types same as
static_assert(
	is::type<
		I3::indices_of_same_as<int>
	>::template same_as<
		indices::of<0,1,2>
	>
);

static_assert(
	is::type<
		I3::indices_of_same_as<float>
	>::template same_as<
		indices::of<>
	>
);

static_assert(
	is::type<
		IFB::indices_of_same_as<float>
	>::template same_as<
		indices::of<1>
	>
);

// indices of types that are args fot invocable
using int_consumer = decltype([](int){});

static_assert(
	I3::indices_of_args_for_invocable_type<
		int_consumer
	>::size == 3
);

// erase at index
static_assert(
	is::type<
		IFB::erase_at_index<2>
	>::template same_as<
		types::of<int, float>
	>
);

// erase at indices
static_assert(
	is::type<
		IFB::erase_at_indices<1, 2>
	>::template same_as<
		types::of<int>
	>
);

// index of first type
/*static_assert(
	IFB::index_of_first_type<float> == 1
);*/

// count
static_assert(I3::count_of_same_as<int> == 3);
static_assert(IFB::count_of_same_as<int> == 1);
static_assert(IFB::count_of_same_as<float> == 1);
static_assert(IFB::count_of_same_as<bool> == 1);
static_assert(IFB::count_of_same_as<char> == 0);

// contains type
static_assert(IFB::contains_same_as<bool>);
static_assert(!IFB::contains_same_as<char>);

int main() {}