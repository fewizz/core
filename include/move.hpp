#pragma once

#include "./type/remove_reference.hpp"

template<typename Type>
constexpr remove_reference<Type>&& move(Type&& v) {
	return (remove_reference<Type>&&) v; 
}