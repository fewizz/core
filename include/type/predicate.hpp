#pragma once

namespace type {

	struct predicate_marker {};

	template<typename Type>
	concept predicate = __is_base_of(type::predicate_marker, Type);

}; // type