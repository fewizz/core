#pragma once

#include "declaration.hpp"

template<typename Type, Type DefaultValue>
struct handle_base {
	Type m_handle_value{ DefaultValue };

	Type& value() {
		return m_handle_value;
	}

	Type& value() const {
		return m_handle_value;
	}

};