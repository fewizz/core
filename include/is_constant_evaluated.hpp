#pragma once

inline constexpr bool is_constant_evaluated() {
	return __builtin_is_constant_evaluated();
}