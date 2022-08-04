#pragma once

#include "type/remove_reference.hpp"

constexpr auto&& move(auto&& v) {
	return (remove_reference<decltype(v)>&&) v; 
}