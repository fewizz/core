#pragma once

namespace value {

	struct predicate_marker {};

	template<typename Type>
	concept predicate = __is_base_of(value::predicate_marker, Type);

} // vluae