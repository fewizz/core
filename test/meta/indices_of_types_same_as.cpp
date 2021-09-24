#include "meta/indices_of_types_same_as.hpp"
#include "meta/count_of_types_same_as.hpp"
#include "meta/are_types_same.hpp"

using IFB = types_of<int, float, int, bool>;

using indices0 = IFB::template pass_to<indices_of_types_same_as<int>::for_types_of>;

static_assert(
	are_types_same<indices0, indices_of<0u, 2u>>
);

static constexpr auto ints_count =
	IFB::template pass_to<count_of_types_same_as<int>::template for_types_of_t>::value;

static_assert(
	ints_count == 2u
);

int main() {}