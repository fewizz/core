#pragma once

#include "basic.hpp"

namespace range {

	template<typename Type>
	struct contains {
		Type&& t;

		contains(Type&& t) : t{ forward<Type>(t) } {}

		template<range::basic R>
		constexpr bool operator () (R&& r) const {
			for(auto&& v : forward<R>(r)) {
				if(v == t) return true;
			}
			return false;
		}
	};

	template<typename Type>
	contains(Type&&) -> contains<Type&&>;
}