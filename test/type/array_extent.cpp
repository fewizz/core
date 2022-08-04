#include <type/array_extent.hpp>

static_assert(array_extent<int[3], 0> == 3);
static_assert(array_extent<int[3], 0> != 2);

int main() {}