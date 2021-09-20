#pragma once

#include "../range.hpp"
#include "same.hpp"

template<typename Type>
struct is_range {

	template<typename ValueType>
	static constexpr bool of =
		range<Type> &&
		are_same<typename Type::value_type, ValueType>
	;

	template<typename ValueType>
	using of_predicate = value::of<is_range<Type>::of<ValueType>>;
};

template<typename Type>
struct indices_of_ranges_of {

	template<typename... Types>
	using of_types = indices_of_satisfying<
		is_range<Types...>::template of
	>;

};