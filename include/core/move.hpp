#pragma once

#include "type/remove_reference.hpp"

auto&& move(auto&& v) {
	using type = type::remove_reference::for_type<decltype(v)>;
	return (type&&) v;
}