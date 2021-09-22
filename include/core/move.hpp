#pragma once

#include "meta/remove_reference.hpp"

auto&& move(auto&& v) {
	using type = remove_reference_type<decltype(v)>;
	return (type&&) v;
}