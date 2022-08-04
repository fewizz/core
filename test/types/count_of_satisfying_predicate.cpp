#include <types/count_of_satisfying_predicate.hpp>
#include <type/is_same_as.hpp>

static_assert(
	types::count_of_satisfying_predicate<
		type::is_same_as<int>
	>::for_types<int, float, int, int, bool>
	== 3
);

int main() {}