#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_function = __is_function(Type);

}