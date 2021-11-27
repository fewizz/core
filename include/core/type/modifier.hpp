#pragma once

namespace type {

	template<typename Type>
	concept modifier = Type::is_type_modifier == true;
}