#include "meta/values/of.hpp"

using zero_size = indices::from<0>::to<0>;

static_assert(zero_size::size == 0);

using size_100 = indices::from<0>::to<100>;

static_assert(size_100::size == 100);

int main() {

}