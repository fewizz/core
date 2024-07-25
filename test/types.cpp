#include <types.hpp>
#include <__type/is_same_as.hpp>

static_assert(same_as<
	types<int, float>::template get<is_same_as<int> && is_same_as<int>>,
	int
>);

int main() {}