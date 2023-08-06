#pragma once

#include "./__type/remove_reference.hpp"

template<typename Type>
constexpr Type&& forward(remove_reference<Type>& v) {
	return (Type&&) v;
}

template<typename Type>
constexpr Type&& forward(remove_reference<Type>&& v) {
	return (Type&&) v;
}