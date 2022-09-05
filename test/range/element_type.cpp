#include <range.hpp>
#include <type/is_same_as.hpp>

static_assert(same_as<range::element_type<int[10]>, int&>);

int main() {}