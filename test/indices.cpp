#include "indices.hpp"
#include "is.hpp"

using i0 = indices<0, 2, 3, 4>;

static_assert(i0::at_index<0> == 0);
static_assert(i0::at_index<1> == 2);
static_assert(i0::at_index<2> == 3);
static_assert(i0::at_index<3> == 4);

using i1 = typename from<0>::to<3>;
static_assert(is::type<i1>::template same_as<indices<0, 1, 2>>);

static_assert(is::type<i0>::template same_as<typename i0::at_indices<0, 1, 2, 3>>);

static_assert(is::type<i0::erase_at_index<0>>::template same_as<indices<2, 3, 4>>);