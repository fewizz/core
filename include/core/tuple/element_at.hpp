#pragma once

#include "../integer.hpp"
#include "../forward.hpp"

template<uint Index>
struct element_at {

	template<typename Head, typename... TypesTail>
	static constexpr decltype(auto) of(Head&& head, TypesTail&&... values) {
		if constexpr(Index == 0u) {
			return forward<Head>(head);
		}
		else {
			static_assert(sizeof...(TypesTail) > 0);
			return of(forward<TypesTail>(values)...);
		}
	}

};