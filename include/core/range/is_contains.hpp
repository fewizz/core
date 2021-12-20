#pragma once

#include "basic.hpp"

namespace range {

	template<typename Type>
	struct is_contains {
		Type&& t;

		is_contains(Type&& t) : t{ forward<Type>(t) } {}

		template<range::basic R>
		constexpr bool for_range(R&& r) const {
			for(auto&& v : forward<R>(r)) {
				if(v == t) return true;
			}
			return false;
		}
	};

	template<typename Type>
	is_contains(Type&&) -> is_contains<Type&&>;
}