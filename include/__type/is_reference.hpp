#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_reference = __is_reference(Type);

	template<typename Type>
	concept reference = __is_reference(Type);

	template<typename Type>
	concept not_a_reference = !reference<Type>;

}

template<typename Type>
constexpr inline bool type_is_reference = __is_reference(Type);