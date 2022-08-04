#pragma once

namespace types {
	
	struct predicate_marker {};

	template<typename Type>
	concept predicate = __is_base_of(types::predicate_marker, Type);
	
} // types