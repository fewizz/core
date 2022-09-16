#pragma once

#include "../__range/extensions.hpp"
#include "../forward.hpp"

template<typename Type>
struct storage : range_extensions<storage<Type>> {
	alignas(Type) uint8 data[sizeof(Type)];

	constexpr auto iterator() const { return data; }
	constexpr auto iterator()       { return data; }

	constexpr auto sentinel() const { return data + sizeof(Type); }
	constexpr auto sentinel()       { return data + sizeof(Type); }

	template<typename... Args>
	void construct(Args&&... args) {
		new (data) Type(forward<Args>(args)...);
	}

	void destruct() {
		((Type*)data)->~Type();
	}
};