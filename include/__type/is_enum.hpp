#pragma once

namespace __type {

	template<typename Type>
	constexpr inline bool is_enum = __is_enum(Type);

}