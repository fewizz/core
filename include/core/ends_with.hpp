#pragma once

#include <ranges>
#include <type_traits>

namespace u {

// TODO replace with std::ranges::ends_with when implemented
template<std::ranges::sized_range R0, std::ranges::sized_range R1>
bool ends_with(R0&& r0, R1&& r1) {
	auto s0 = r0.size();
	auto s1 = r1.size();

	if(s1 > s0) return false;

	auto iter = r0.begin();
	std::advance(iter, s0 - s1);

	return std::equal(iter, r0.end(), r1.begin());
}

template<std::ranges::sized_range R0, typename... Ts>
bool ends_with(R0&& r0, Ts... ts) {
	return u::ends_with(
		r0,
		std::array<std::common_type_t<Ts...>, sizeof...(Ts)>{ ts... }
	);
}

}