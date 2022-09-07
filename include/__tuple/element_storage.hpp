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

	constexpr auto& get() const {
		return element_;
	}

	constexpr auto& get() {
		return element_;
	}

	constexpr decltype(auto) forward() const {
		return ::forward<Type>(element_);
	}

	constexpr decltype(auto) forward() {
		return ::forward<Type>(element_);
	}

};

}