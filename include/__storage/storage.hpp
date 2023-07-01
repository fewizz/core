#pragma once

#include "../__range/extensions.hpp"
#include "../__type/is_constructible_from.hpp"
#include "../__type/is_trivial.hpp"
#include "../__type/is_reference.hpp"
#include "../forward.hpp"
#include "../move.hpp"

template<nuint Size, nuint Alignment>
struct storage_of_size_and_alignment : range_extensions<uint1a> {
	alignas(Alignment) uint1a data[Size];

	template<typename Type>
	static constexpr bool storable =
		Size >= sizeof(Type) &&
		Alignment % alignof(Type) == 0;

	constexpr auto iterator() const { return data; }
	constexpr auto sentinel() const { return data + Size; }

	template<typename Type, typename... Args>
	requires
		constructible_from<Type, Args...> &&
		storable<Type>
	Type& construct(Args&&... args) {
		Type* ptr = new (data) Type(
			forward<Args>(args)...
		);
		return *ptr;
	}

	template<typename Type>
	requires storable<Type>
	void destruct() {
		((Type*)data)->~Type();
	}

	template<typename Type>
	requires storable<Type>
	const Type&& move() const {
		const Type& e = *(const Type*) data;
		return ::move(e);
	}
	template<typename Type>
	requires storable<Type>
	Type&& move() {
		Type& e = *(Type*) data;
		return ::move(e);
	}

	template<typename Type>
	requires storable<Type>
	const Type&  get() const &  { return *(Type*) data; }
	template<typename Type>
	requires storable<Type>
	      Type&  get()       &  { return *(Type*) data; }

	template<typename Type>
	requires storable<Type>
	const Type&& get() const && { return move(); }
	template<typename Type>
	requires storable<Type>
	      Type&& get()       && { return move(); }

};

template<typename Type>
struct storage : storage_of_size_and_alignment<sizeof(Type), alignof(Type)> {
	using base_type
		= storage_of_size_and_alignment<sizeof(Type), alignof(Type)>;
	using type = Type;

	template<typename... Args>
	Type& construct(Args&&... args) {
		return base_type::template
			construct<Type, Args...>(forward<Args>(args)...);
	}

	void destruct() {
		base_type::template destruct<Type>();
	}

	const Type&& move() const {
		return base_type::template move<Type>();
	}
	      Type&& move()       {
		return base_type::template move<Type>();
	}

	const Type&  get() const &  { return base_type::template get<Type>(); }
	      Type&  get()       &  { return base_type::template get<Type>(); }

	const Type&& get() const && { return move(); }
	      Type&& get()       && { return move(); }

};