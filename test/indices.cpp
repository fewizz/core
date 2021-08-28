#include "indices.hpp"
#include "is.hpp"
#include "erase_at.hpp"

using i0 = indices::of<0, 2, 3, 4>;

static_assert(i0::at<0> == 0);
static_assert(i0::at<1> == 2);
static_assert(i0::at<2> == 3);
static_assert(i0::at<3> == 4);

using i1 = typename indices::from<0>::to<3>;
static_assert(is::type<i1>::template same_as<indices::of<0, 1, 2>>);

//static_assert(is::type<i0>::template same_as<typename i0::at_indices<0, 1, 2, 3>>);
static_assert(is::type<erase_at<0>::of_values<0ul, 2ul, 3ul, 4ul>>::template same_as<indices::of<2, 3, 4>>);