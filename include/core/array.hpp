#pragma once

#include "integer.hpp"

template<typename Type, uint Size>
struct array {
	Type m_array[Size];

	auto begin() const {
		return m_array;
	}

	auto end() const {
		return m_array + Size;
	}
};