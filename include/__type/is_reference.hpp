#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_reference = __is_reference(Type);

}