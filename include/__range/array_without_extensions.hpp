#pragma once

#include "integer.hpp"
#include "../__types/are_same.hpp"
#include "../__types/first.hpp"
#include "../__type/remove_reference.hpp"

namespace __range {

template<typename Type, nuint Size>
struct array_without_extensions {
private:
	Type array_[Size];
public:

	template<typename... Args>
	constexpr array_without_extensions(Args&&... args) :
		array_{ forward<Args>(args)... }
	{}

	constexpr nuint size() const {
		return Size;
	}

	constexpr const Type* iterator() const & { return array_; }
	constexpr       Type* iterator()       & { return array_; }

	constexpr const Type* sentinel() const & { return array_ + Size; }
	constexpr       Type* sentinel()       & { return array_ + Size; }
};

template<typename... Types>
requires(sizeof...(Types) == 1 || types_are_same<Types...>)
array_without_extensions(Types&&...)
	-> array_without_extensions<
		remove_reference<first_type<Types...>>,
		sizeof...(Types)
	>;

}