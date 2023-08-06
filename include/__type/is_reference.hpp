#pragma once

#include "./remove_reference.hpp"

namespace __type {

	template<typename Type>
	constexpr inline bool is_reference = __is_reference(Type);

	template<typename Type>
	concept reference = __is_reference(Type);

	template<typename Type>
	concept not_a_reference = !reference<Type>;

	template<typename Type>
	constexpr inline bool is_lvalue_reference =
		__is_same_as(Type, remove_reference<Type>&);

	template<typename Type>
	concept lvalue_reference = __is_same_as(Type, remove_reference<Type>&);

	template<typename Type>
	constexpr inline bool is_rvalue_reference =
		__is_same_as(Type, remove_reference<Type>&&);

	template<typename Type>
	concept rvalue_reference = __is_same_as(Type, remove_reference<Type>&&);

}

template<typename Type>
constexpr inline bool type_is_reference = __is_reference(Type);

template<typename Type>
constexpr inline bool type_is_lvalue_reference =
	__type::is_lvalue_reference<Type>;

template<typename Type>
constexpr inline bool type_is_rvalue_reference =
	__type::is_rvalue_reference<Type>;