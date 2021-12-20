#pragma once

#include "type/remove_reference.hpp"

template<typename T>
constexpr T&& forward(type::remove_reference::for_type<T>& t) {
	return (T&&)t;
}

template<typename T>
constexpr T&& forward(type::remove_reference::for_type<T>&& t) {
	return (T&&)t;
}