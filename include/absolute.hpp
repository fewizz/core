#pragma once

constexpr auto absolute(auto x) {
	return x < 0 ? -x : x;
}