#pragma once

#include <type_traits>
#include <concepts>

namespace u {

template<typename T>
struct named : T {};

template<typename T>
requires(std::is_arithmetic_v<T> || std::is_pointer_v<T>)
struct named<T> {
	T value;
};

} // u