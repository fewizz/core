#pragma once

#include "../span.hpp"

namespace range {

	template<typename Type>
	struct view_on_stack {
		using value_type = Type;
		nuint m_count;

		view_on_stack(nuint count) :
			m_count{ count }
		{}

		template<typename Handler>
		decltype(auto) operator () (Handler&& handler) {
			value_type storage[m_count];
			return handler(span{ storage, m_count });
		}

	};

} // range