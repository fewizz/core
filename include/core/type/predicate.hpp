#pragma once

namespace type {

	template<typename Type>
	concept predicate = Type::is_type_predicate == true;
};