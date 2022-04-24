#pragma once

#include "span.hpp"

template<typename Type>
class view_on_stack {
	nuint m_count;
public:

	using value_type = Type;

	view_on_stack(nuint count) :
		m_count{ count }
	{}

	template<typename Handler>
	decltype(auto) operator () (Handler&& handler) {
		value_type storage[m_count];
		return handler(span{ storage, m_count });
	}

};