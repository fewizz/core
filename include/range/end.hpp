#pragma once

#include "integer.hpp"

namespace range {

	struct end_t {

		template<typename Type, nuint Size>
		constexpr auto operator () (Type (&o)[Size]) const {
			return o + Size;
		}

		template<typename Type>
		requires requires (Type o) { o.end(); }
		constexpr auto operator () (Type&& o) const {
			return o.end();
		}

	};

	inline constexpr end_t end {};

} // range