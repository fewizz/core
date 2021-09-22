#pragma once

#include "meta/remove_reference.hpp"

template<typename T>
constexpr T&& forward(remove_reference<T>& t) {
	return (T&&)t;
}

template<typename T>
constexpr T&& forward(remove_reference<T>&& t) {
	return (T&&)t;
}