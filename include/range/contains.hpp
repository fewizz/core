#pragma once

#include "./basic.hpp"

namespace range {

	template<typename ValueType>
	struct contains {
		ValueType& value;

		constexpr contains(ValueType& value) : value{ value } {}

		template<basic_range Range>
		constexpr bool operator () (Range&& range) const {
			for(auto&& v : forward<Range>(range)) {
				if(v == value) return true;
			}
			return false;
		}

		template<basic_range Range>
		constexpr bool in(Range&& range) const {
			return (*this)(forward<Range>(range));
		}
	};

	template<typename Type>
	contains(Type&&) -> contains<Type&&>;

} // range