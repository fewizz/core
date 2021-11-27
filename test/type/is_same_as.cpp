#include "type/is_same_as.hpp"
#include "type/remove_reference.hpp"

static_assert(
	type::is_same_as<int>
		::after_applying<type::remove_reference>
		::for_type_of<int&>
);

int main() {}