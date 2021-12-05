#include "type/is_same_as.hpp"
#include "type/remove_reference.hpp"
#include "type/remove_const.hpp"
#include "integer.hpp"

static_assert(
	!type::is_same_as<uint32>::for_type_of<bool>
);

static_assert(
	type::is_same_as<int>
		::after_applying<type::remove_reference>
		::for_type_of<int&>
);

static_assert(
	type::is_same_as<int>
		::after_applying<type::remove_const>
		::after_applying<type::remove_reference>
		::for_type_of<const int&>
);

int main() {}