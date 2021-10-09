#pragma once

namespace value {

	template<typename Type>
	concept predicate = Type::is_value_predicate == true;
}