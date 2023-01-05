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
	constexpr const Type&& get0() const && {
		return forward<const Type>(element_);
	}
	constexpr       Type&& get0()       && {
		return forward<      Type>(element_);
	}

	constexpr const Type&& forward() const {
		return ::forward<const Type>(element_);
	}

	constexpr       Type&& forward() {
		return ::forward<      Type>(element_);
	}

};

}