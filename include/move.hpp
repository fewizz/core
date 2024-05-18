#pragma once

#include "./__type/remove_reference.hpp"

template<typename Type>
inline constexpr remove_reference<Type>&& move(Type&& v) {
	return (remove_reference<Type>&&) v; 
}