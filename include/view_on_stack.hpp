#pragma once

#include "./span.hpp"

template<typename Type>
class view_on_stack {
	nuint count_;
public:

	view_on_stack(nuint count) :
		count_{ count }
	{}

	template<typename Handler>
	decltype(auto) operator () (Handler&& handler) const {
		Type storage[count_];
		return handler(span<Type>{ storage, count_ });
	}

};