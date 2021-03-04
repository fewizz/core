#pragma once

#include <concepts>
#include <type_traits>

namespace u {

	template<class T>
	struct div_t {
		T quot;
		T rem;

		constexpr div_t(T q, T r) : quot{q}, rem{r} {}
	};

	template<std::integral Int>
	constexpr div_t<Int> div(Int a, Int b) {
		return { a / b, a % b };
	}

	template<std::integral Int>
	constexpr auto div_floor(Int a, Int b) {
		auto div_res = u::div(a, b);

		if(a < 0 && div_res.rem != 0) {
			--div_res.quot;
			div_res.rem += b;
		}

		return div_res;
	}

}