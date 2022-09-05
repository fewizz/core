#pragma once

#include "./__type/remove_reference.hpp"

template<typename Type>
constexpr __type::remove_reference<Type>&& move(Type&& v) {
	return (__type::remove_reference<Type>&&) v; 
}