#pragma once

#include "./declaration.hpp"
#include "../range.hpp"

struct c_string_sentinel{};

constexpr bool operator == (c_string_sentinel, const auto* ptr) {
	return *ptr == 0;
}

constexpr bool operator == (const auto* ptr, c_string_sentinel) {
	return *ptr == 0;
}

template<typename Type>
struct c_string<c_string_type::unknown_size, Type> :
	range_extensions<c_string<c_string_type::unknown_size, Type>>
{
private:
	const Type* ptr_;
public:

	static constexpr bool is_borrowed_range = true;

	constexpr c_string(const Type* ptr) : ptr_{ ptr } {}

	constexpr const Type*       iterator() const { return ptr_; }
	constexpr c_string_sentinel sentinel() const { return {}; }

	constexpr const Type* elements_ptr() const { return ptr_; }

	constexpr const Type& operator [] (nuint index) const {
		return ptr_[index];
	}

	constexpr c_string<c_string_type::known_size, Type> sized() const {
		return {
			ptr_,
			__iterator_and_sentinel::get_or_compute_distance(
				iterator(),
				sentinel()
			)
		};
	}

};

template<typename Type>
requires
	type<Type>::is_pointer
c_string(Type) -> c_string<
	c_string_type::unknown_size,
	remove_const<remove_pointer<Type>>
>;