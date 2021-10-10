#pragma once

namespace types {
	
	template<typename Type>
	concept predicate = Type::is_types_predicate;
	
};