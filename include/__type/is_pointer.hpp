#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_pointer = __is_pointer(Type);

}

template<typename Type>
constexpr inline bool type_is_pointer = __is_pointer(Type);