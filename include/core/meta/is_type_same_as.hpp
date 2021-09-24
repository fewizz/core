#pragma once

#include "are_types_same.hpp"
#include "value.hpp"

template<typename Type0>
struct is_type_same {
	template<typename Type1>
	static constexpr bool as = are_types_same<Type0, Type1>;

	template<typename Type1>
	using as_t = value_of<is_type_same<Type0>::as<Type1>>;
};