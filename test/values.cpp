#include "values.hpp"
#include "is.hpp"
#include "erase_at.hpp"

using i0 = indices::of<0, 2, 3, 4>;

static_assert(i0::value_at_index<0> == 0);
static_assert(i0::value_at_index<1> == 2);
static_assert(i0::value_at_index<2> == 3);
static_assert(i0::value_at_index<3> == 4);

static_assert(i0::contains_value<0>);
static_assert(i0::contains_value<2>);
static_assert(i0::contains_value<3>);
static_assert(i0::contains_value<4>);

static_assert(is::type<i0::indices_of_values_same_as<0>>::template same_as<indices::of<0>>);
static_assert(is::type<i0::indices_of_values_same_as<2>>::template same_as<indices::of<1>>);
static_assert(is::type<i0::indices_of_values_same_as<3>>::template same_as<indices::of<2>>);
static_assert(is::type<i0::indices_of_values_same_as<4>>::template same_as<indices::of<3>>);

using i1 = typename indices::from<0>::to<3>;
static_assert(is::type<i1>::template same_as<indices::of<0, 1, 2>>);

static_assert(is::type<erase_at<0>::of_values<0ul, 2ul, 3ul, 4ul>>::template same_as<indices::of<2, 3, 4>>);
static_assert(is::type<i0::erase_value_at_index<0>>::template same_as<indices::of<2, 3, 4>>);

static_assert(
	is::type<
		i0::template erase_values_at<indices::of<0>>
	>::template same_as<indices::of<2,3,4>>
);

static_assert(
	is::type<
		i0::template erase_values_at<indices::of<0, 1>>
	>::template same_as<indices::of<3,4>>
);

static_assert(
	is::type<
		i0::template erase_values_at<indices::of<1, 0>>
	>::template same_as<indices::of<3,4>>
);

static_assert(
	is::type<
		i0::template erase_values_at<indices::of<0, 1, 3, 2>>
	>::template same_as<indices::of<>>
);

int main() {}