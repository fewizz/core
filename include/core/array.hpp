#pragma once

#include "integer.hpp"
#include "types/are_same.hpp"

template<typename Type, uint Size>
struct array {
	using value_type = Type;
	value_type m_array[(primitive::uint)Size];

	constexpr uint size() const {
		return Size;
	}

	constexpr auto begin() const {
		return m_array;
	}

	constexpr auto end() const {
		return m_array + (primitive::uint)Size;
	}

	constexpr value_type* data() {
		return m_array;
	}

	constexpr const value_type* data() const {
		return m_array;
	}
};

template<typename HeadType, typename... TailTypes>
requires(sizeof...(TailTypes) == 0u || types::are_same::for_types_of<HeadType, TailTypes...>)
array(HeadType&&, TailTypes&&...) -> array<HeadType, sizeof...(TailTypes) + 1u>;