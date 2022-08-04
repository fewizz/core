#include <range/value_type.hpp>
#include <type/is_same_as.hpp>

static_assert(same_as<range::value_type<int[10]>, int>);

int main() {}