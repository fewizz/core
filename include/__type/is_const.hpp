#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_const = __is_const(Type);

}

template<typename Type>
inline constexpr bool type_is_const = __is_const(Type);