#pragma once

#include "../type/predicate.hpp"

namespace elements {

	template<type::predicate>
	struct view_satisfying_type_predicate_t {

	};

	template<type::predicate Predicate>
	inline constexpr auto view_satisfying_type_predicate = view_satisfying_type_predicate_t<Predicate>{};

}