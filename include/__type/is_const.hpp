#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_const = __is_const(Type);

}