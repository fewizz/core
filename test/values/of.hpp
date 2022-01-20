#include "values/of.hpp"

using zero_size = indices::from<0>::to<0>;

static_assert(zero_size::size == 0);