#pragma once

namespace type {

	struct modifier_marker {};

	template<typename Type>
	concept modifier = __is_base_of(type::modifier_marker, Type);

}