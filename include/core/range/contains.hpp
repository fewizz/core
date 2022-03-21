#pragma once

#include "basic.hpp"

namespace range {

	template<typename ValueType>
	struct contains {
		ValueType& value;

		constexpr contains(ValueType& value) : value{ value } {}

		template<range::basic R>
		constexpr bool operator () (R&& r) const {
			for(auto&& v : forward<R>(r)) {
				if(v == value) return true;
			}
			return false;
		}

		template<range::basic R>
		constexpr bool in(R&& r) const {
			return (*this)(forward<R>(r));
		}
	};

	template<typename Type>
	contains(Type&&) -> contains<Type&&>;
}