#pragma once

template<typename Type>
constexpr Type bit_cast(auto value) {
	return __builtin_bit_cast(Type, value);
}