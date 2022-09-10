#include <c_string.hpp>
#include <__range/sized.hpp>

static_assert(!sized_range<c_string<c_string_type::unknown_size>>);

int main() {}