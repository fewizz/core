#pragma once

#include "./span.hpp"

template<typename Type>
class view_on_stack {
	nuint count_;
public:

	constexpr view_on_stack(nuint count) :
		count_{ count }
	{}

	template<typename Handler>
	constexpr decltype(auto) operator () (Handler&& handler) const {
		if(count_ > 0) {
			Type storage[count_];
			return handler(span<Type>{ storage, count_ });
		}
		else {
			return handler(span<Type>{});
		}
	}

};