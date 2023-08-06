#include <__range/element_type.hpp>
#include <__type/is_same_as.hpp>

static_assert(same_as<range_element_type<int[10]>, int&>);

int main() {}