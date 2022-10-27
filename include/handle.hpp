#pragma once

#include <forward.hpp>

template<typename UnderlyingType>
struct handle_base {
protected:
	UnderlyingType value_;

	handle_base(UnderlyingType type) :
		value_{ type }
	{}

public:

	      UnderlyingType& underlying()       { return value_; }
	const UnderlyingType& underlying() const { return value_; }

};

template<typename Type>
struct handle;