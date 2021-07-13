#pragma once

#include <concepts>
#include <type_traits>

namespace u {

	template<typename T>
	struct div_t {
		T quot;
		std::make_unsigned_t<T> rem;

		constexpr div_t(T q, std::make_unsigned_t<T> r) : quot{ q }, rem{ r } {}
	};

	constexpr auto div(std::integral auto a, std::integral auto b) {
		return div_t{ a / b, a % b };
	}

	constexpr auto div_floor(std::integral auto a, std::unsigned_integral auto b) {
		if(a < 0) {
			auto div_res = u::div(-a, b);
			if(div_res.rem == 0) return div_t{ - div_res.quot, 0 };
			return div_t{ - div_res.quot - 1, b - div_res.rem };
		}

		return u::div(a, b);
	}

}