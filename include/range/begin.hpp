#pragma once

#include "integer.hpp"
#include "type/is_array.hpp"
#include "type/remove_const.hpp"
#include "type/remove_reference.hpp"

namespace range {

	struct begin_t {

		template<typename Type>
		requires is_array<remove_const<remove_reference<Type>>>
		constexpr auto operator () (Type&& o) const {
			return o;
		}

		template<typename Type>
		requires requires (Type&& o) { o.begin(); }
		constexpr auto operator () (Type&& o) const {
			return o.begin();
		}

	};

	inline constexpr begin_t begin {};

} // range