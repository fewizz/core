#pragma once

#include <concepts>
#include <type_traits>
#include <cstdlib>
#include <cinttypes>

namespace u {

	template<class T>
	struct div_t {
		T quot;
		T rem;
	};

	template<std::unsigned_integral UInt>
	div_t<UInt> div(UInt a, UInt b) {
		return { a / b, a % b };
	}

	inline auto div(int a, int b) {
		return std::div(a, b);
	}

	inline auto div(long int a, long int b) {
		return std::ldiv(a, b);
	}

	inline auto div(long long int a, long long int b) {
		return std::lldiv(a, b);
	}

	template<std::integral Int>
	auto div_floor(Int a, Int b) {
		auto div_res = u::div(a, b);

		if(a < 0 && div_res.rem != 0) {
			--div_res.quot;
			div_res.rem += b;
		}

		return div_res;
	}

}