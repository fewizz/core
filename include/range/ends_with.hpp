#pragma once

#include "./basic.hpp"
#include "./value_type.hpp"
#include "../ranges/equals.hpp"
#include "../array.hpp"
#include "../types/are_same.hpp"
#include "../types/first.hpp"

namespace range {

	template<basic_range Range>
	struct ends {
		Range range_;

		constexpr ends(Range&& range) : range_{ forward<Range>(range) } {}

		template<basic_range OtherRange>
		constexpr bool with(OtherRange&& other) const {
			auto size = range_.size();
			auto other_size = other.size();

			if(size < other_size) return false;

			return equals(
				to_range(
					range_.begin() + (size - other_size), range_.end()
				),
				other
			);
		}

		template<typename... Types>
		requires(
			(sizeof...(Types) == 1 || types_are_same<decay<Types>...>) &&
			types_are_same<
				decay<element_type<Range>>,
				decay<first_type<Types...>>
			>
		)
		constexpr bool with(Types&&... values) const {
			return with(array{ values... });
		}

	};

	template<basic_range Range>
	ends(Range&&) -> ends<Range>;

}