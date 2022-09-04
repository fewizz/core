#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_pointer = __is_pointer(Type);

}