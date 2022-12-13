#pragma once

#include "../forward.hpp"
#include "../integer.hpp"

namespace __tuple {

template<nuint Index, typename Type>
class element_storage {
	Type element_;
public:

	constexpr element_storage(Type&& element) :
		element_{ ::forward<Type>(element) }
	{}

	// get0 instead of get, to disambiguate structured binding
	constexpr const Type&  get0() const &  { return element_; }
	constexpr       Type&  get0()       &  { return element_; }
	constexpr const Type&& get0() const && { return move(element_); }
	constexpr       Type&& get0()       && { return move(element_); }

	constexpr decltype(auto) forward() const {
		return ::forward<Type>(element_);
	}

	constexpr decltype(auto) forward() {
		return ::forward<Type>(element_);
	}

};

}