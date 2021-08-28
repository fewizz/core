#include "values.hpp"
#include "is.hpp"
#include "erase_at.hpp"

using i0 = indices::of<0, 2, 3, 4>;

static_assert(i0::at<0> == 0);
static_assert(i0::at<1> == 2);
static_assert(i0::at<2> == 3);
static_assert(i0::at<3> == 4);

static_assert(i0::contains<0>);
static_assert(i0::contains<2>);
static_assert(i0::contains<3>);
static_assert(i0::contains<4>);

static_assert(is::type<i0::indices_of_same_as<0>>::template same_as<indices::of<0>>);
static_assert(is::type<i0::indices_of_same_as<2>>::template same_as<indices::of<1>>);
static_assert(is::type<i0::indices_of_same_as<3>>::template same_as<indices::of<2>>);
static_assert(is::type<i0::indices_of_same_as<4>>::template same_as<indices::of<3>>);

using i1 = typename indices::from<0>::to<3>;
static_assert(is::type<i1>::template same_as<indices::of<0, 1, 2>>);

static_assert(is::type<erase_at<0>::of_values<0ul, 2ul, 3ul, 4ul>>::template same_as<indices::of<2, 3, 4>>);
static_assert(is::type<i0::erase_at<0>>::template same_as<indices::of<2, 3, 4>>);