#pragma once

#include "./__iterator_and_sentinel/distance.hpp"
#include "./type/remove_pointer.hpp"
#include "./type/remove_const.hpp"
#include "./type/is_pointer.hpp"
#include "./type/is_const.hpp"

struct c_string_sentinel{};

constexpr bool operator == (c_string_sentinel, const auto* ptr) {
	return *ptr == 0;
}

constexpr bool operator == (const auto* ptr, c_string_sentinel) {
	return *ptr == 0;
}

enum class c_string_type {
	known_size, unknown_size
};

template<c_string_type Sized, typename Type = char>
struct c_string;

template<typename Type>
struct c_string<c_string_type::unknown_size, Type> {
private:
	const Type* ptr_;
public:

	constexpr c_string(const Type* ptr) : ptr_{ ptr } {}

	constexpr const Type*       iterator() const { return ptr_; }
	constexpr c_string_sentinel sentinel() const { return {}; }

	constexpr const Type* elements_ptr() const { return ptr_; }

	constexpr const Type& operator [] (nuint index) const {
		return ptr_[index];
	}

	constexpr c_string<c_string_type::known_size, Type> sized() const {
		return {
			ptr_, __iterator_and_sentinel::distance(iterator(), sentinel())
		};
	}

};

template<typename Type>
struct c_string<c_string_type::known_size, Type> {

private:
	const Type* ptr_;
	nuint size_;
public:

	constexpr c_string() = default;

	template<nuint Size>
	constexpr c_string(const Type (&array)[Size]) :
		ptr_{ array }, size_{ Size - 1 }
	{}

	constexpr c_string(const Type* ptr, nuint size) :
		ptr_{ ptr }, size_{ size }
	{}

	template<nuint Size>
	constexpr c_string& operator = (const Type* (&array)[Size]) {
		ptr_ = array;
		size_ = Size - 1;
		return *this;
	}

	constexpr const Type* iterator() const { return ptr_; }
	constexpr const Type* sentinel() const { return ptr_ + size_; }

	constexpr const Type* elements_ptr() const { return ptr_; }

	constexpr const Type& operator [] (nuint index) const {
		return ptr_[index];
	}

	constexpr nuint size() const {
		return size_;
	}
};

template<typename Type, nuint Size>
c_string(Type(&)[Size]) -> c_string<
	c_string_type::known_size,
	Type
>;

template<typename Type>
c_string(const Type*, nuint size) -> c_string<
	c_string_type::known_size,
	Type
>;

template<typename Type>
requires is_pointer<Type> && is_const<remove_pointer<Type>>
c_string(Type) -> c_string<
	c_string_type::unknown_size,
	remove_const<remove_pointer<Type>>
>;