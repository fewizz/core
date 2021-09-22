#pragma once

#include "are_same.hpp"
#include "../value.hpp"

template<typename Type0>
struct is_same {
	template<typename Type1>
	static constexpr bool as = are_types_same<Type0, Type1>;

	template<typename Type1>
	using as_predicate = value::of<is_same<Type0>::as<Type1>>;
};