#pragma once

#include "../__range/extensions.hpp"
#include "../__type/is_constructible_from.hpp"
#include "../forward.hpp"
#include "../move.hpp"

template<typename Type>
struct storage : range_extensions<storage<Type>> {
	alignas(Type) uint8 data[sizeof(Type)];

	constexpr auto iterator() const { return data; }
	constexpr auto sentinel() const { return data + sizeof(Type); }

	template<typename... Args>
	requires constructible_from<Type, Args...>
	Type& construct(Args&&... args) {
		Type* ptr = new (data) Type(forward<Args>(args)...);
		return *ptr;
	}

	void destruct() {
		((Type*)data)->~Type();
	}

	Type&& move() {
		Type& e = *(Type*) data;
		return ::move(e);
	}

	const Type&  get() const &  { return *(Type*)data; }
	      Type&  get()       &  { return *(Type*)data; }

	const Type&& get() const && { return move(); }
	      Type&& get()       && { return move(); }
};