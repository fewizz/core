#pragma once

#include <type_traits>
#include <utility>
#include "indices_of.hpp"

namespace u {

template<auto V>
struct erase {
	template<typename F0>
	struct from;

	template<typename FT, FT... FVs>
	struct from<std::integer_sequence<FT, FVs...>>;

	template<typename FT, FT Fv0, FT... FVs>
	struct from<std::integer_sequence<FT, Fv0, FVs...>> {
		using type
			= std::conditional_t<
				is<FT>::contains<Value>
			>;
	};
};

template<auto... Vs>
struct erase<std::integer_sequence<T, V0, Vs...>> {

	template<typename F0>
	struct from;

	template<typename FT, FT... FVs>
	struct from<std::integer_sequence<FT, FVs...>>;

	template<typename FT, FT Fv0, FT... FVs>
	struct from<std::integer_sequence<FT, Fv0, FVs...>> {
		using type = erase<T, Vs...>::
	};
};

}