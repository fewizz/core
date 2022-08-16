#pragma once

#include "./predicate.hpp"

template<typename Type>
concept trivial = __is_trivial(Type);

template<typename Type>
inline constexpr bool is_trivial = trivial<Type>;

namespace type {

	struct is_trivial : type::predicate_marker {

		template<typename Type>
		static constexpr bool for_type = trivial<Type>;

	};

} // type