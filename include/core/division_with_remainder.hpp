/*#pragma once

#include <concepts>
#include <type_traits>
#include <integer.hpp>

template<integer Q, integer R>
struct division_result {
	Q quotient;
	R remainder;

	constexpr division_result(const division_result&) = default;
	constexpr division_result& operator = (const division_result&) = default;

	constexpr division_result(Q q, R r) : quotient{ q }, remainder{ r } {}
};

template<integer A, integer B>
constexpr auto divide_with_remainder(A a, B b) {
	return division_result<A, B>{ a / b, a % b };
}

template<integer A, unsigned_integer B>
constexpr auto divide_floot_with_remainder(A a, B b) {
	if(a < 0) {
		auto div_res = divide_with_remainder<A, B>(-a, b);
		if(div_res.remainder == 0) return division_result<A, B>{ (A)(- div_res.quotient), (B)0 };
		return division_result<A, B>{ (A)(- div_res.quotient - 1), (B)(b - div_res.remainder) };
	}

	return divide_with_remainder<A, B>(a, b);
}*/