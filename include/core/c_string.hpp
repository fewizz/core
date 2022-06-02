#pragma once

#include "span.hpp"
#include "default_sentinel.hpp"
#include "distance.hpp"
#include "meta/type/remove_pointer.hpp"
#include "meta/type/remove_const.hpp"
#include "meta/type/is_pointer.hpp"
#include "meta/type/is_const.hpp"

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
	using value_type = const Type;

private:
	value_type* ptr_;
public:

	c_string(const Type* ptr) : ptr_{ ptr } {}

	constexpr value_type* begin() const { return ptr_; }
	constexpr value_type* data() const { return ptr_; }
	constexpr c_string_sentinel end() const { return {}; }

	constexpr value_type& operator [] (nuint index) const {
		return ptr_[index];
	}

	c_string<c_string_type::known_size, Type> sized() const {
		return { ptr_, distance(begin(), end()) };
	}
};

template<typename Type>
struct c_string<c_string_type::known_size, Type> {
	using value_type = const Type;

private:
	value_type* ptr_;
	nuint size_;
public:

	c_string() = default;

	template<nuint Size>
	c_string(value_type (&array)[Size]) :
		ptr_{ array }, size_{ Size - 1 }
	{}

	c_string(value_type* ptr, nuint size) :
		ptr_{ ptr }, size_{ size }
	{}

	template<nuint Size>
	c_string& operator = (value_type (&array)[Size]) {
		ptr_ = array;
		size_ = Size - 1;
		return *this;
	}

	constexpr value_type* begin() const { return ptr_; }
	constexpr value_type* data() const { return ptr_; }
	constexpr value_type* end() const { return ptr_ + size_; }

	constexpr value_type& operator [] (nuint index) const {
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
requires is_pointer<Type> && is_const<remove_pointer<Type>>
c_string(Type) -> c_string<
	c_string_type::unknown_size,
	remove_const<remove_pointer<Type>>
>;