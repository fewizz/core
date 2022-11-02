#pragma once

#include "./sized.hpp"
#include "../integer.hpp"

namespace __range {

	struct size_t {

		template<typename Type, nuint Size>
		constexpr nuint operator () (Type (&)[Size] ) const {
			return Size;
		}

		template<sized_range Range>
		requires requires(Range r) { r.size(); }
		constexpr auto operator () (Range&& range) const {
			return range.size();
		}

		template<sized_range Range>
		requires (!requires(Range r) { r.size(); })
		constexpr auto operator () (Range&& range) const {
			return range.sentinel() - range.iterator();
		}

	};

} // __range

inline constexpr __range::size_t range_size {};