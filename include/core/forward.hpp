#pragma once

#include "type/remove_reference.hpp"
#include "elements/of.hpp"

template<typename Type>
constexpr Type&& forward(type::remove_reference::for_type<Type>& v) {
	return (Type&&) v;
}

template<typename Type>
constexpr Type&& forward(type::remove_reference::for_type<Type>&& v) {
	return (Type&&) v;
}