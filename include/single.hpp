#pragma once

#include "integer.hpp"
#include "type/remove_reference.hpp"
#include "range.hpp"

template<typename Type>
class single_view {
	Type value_;

	using value_type = remove_reference<Type>;

public:

	constexpr single_view(Type value) :
		value_{ forward<Type>(value) }
	{}

	constexpr const auto* begin() const { return &value_; }
	constexpr auto* begin() { return &value_; }

	constexpr const auto* end() const { return &value_ + 1; }
	constexpr auto* end() { return &value_ + 1; }

	constexpr const auto* data() const { return &value_; }
	constexpr auto* data() { return &value_; }

	constexpr nuint size() const { return 1; }

};

template<typename Type>
single_view(Type&&) -> single_view<Type>;