#pragma once

constexpr auto max(auto v0, auto v1, auto... values) {
	auto r = v0 > v1 ? v0 : v1;
	if constexpr(sizeof...(values) > 0) {
		return max(r, values...);
	}
	return r;
}