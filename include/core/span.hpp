#pragma once

#include "integer.hpp"
#include "meta/type/is_reference.hpp"
#include "meta/type/remove_reference.hpp"

template<typename ValueType, unsigned_integer SizeType = nuint>
class span {
	using value_type = ValueType;
	using size_type = SizeType;

	value_type* values_;
	size_type size_;
public:

	constexpr span() = default;

	constexpr span(value_type* values, size_type size) :
		values_{ values }, size_{ size }
	{}

	constexpr span(size_type size, value_type* values) :
		values_{ values }, size_{ size }
	{}

	constexpr size_type size() const {
		return size_;
	}

	constexpr const value_type* begin() const {
		return values_;
	}

	constexpr value_type* begin() {
		return values_;
	}

	constexpr const value_type* end() const {
		return values_ + size_;
	}

	constexpr value_type* end() {
		return values_ + size_;
	}

	constexpr auto& operator [] (size_type index) {
		return data()[index];
	}

	constexpr const auto& operator [] (size_type index) const {
		return data()[index];
	}

	constexpr auto& data() {
		return values_;
	}

	constexpr const auto& data() const {
		return values_;
	}

	template<typename Type>
	constexpr span<Type> cast() {
		return span<Type>{ (Type*) data(), size() };
	}

	constexpr span cut(size_type size) {
		return span{ data(), size };
	}

};

template<typename ValueType, typename SizeType>
span(ValueType*, SizeType) -> span<ValueType>;

template<typename ValueType, unsigned_integer SizeType>
requires type::is_reference::for_type<ValueType>
class span<ValueType, SizeType> {
	using clear_value_type = remove_reference<ValueType>;
	using value_type = clear_value_type&;
	using size_type = SizeType;

	clear_value_type** values_;
	size_type size_;
public:
	
	class iterator {
		clear_value_type** ptr_;
	public:

		iterator(clear_value_type** ptr) : ptr_{ ptr } {}

		clear_value_type& operator * () const {
			return **ptr_;
		}

		auto& operator ++ () {
			++ptr_;
			return *this;
		}

		auto& operator += (size_type n) {
			ptr_ += n;
			return *this;
		}
	
		auto operator + (size_type n) const {
			return iterator{ *this } += n;
		}
	
		bool operator == (const iterator other) const {
			return ptr_ == other.ptr_;
		}
	};

	constexpr span(clear_value_type** values, size_type size)
		: values_{ values }, size_{ size }
	{}

	constexpr span(size_type size, clear_value_type** values)
		: values_{ values }, size_{ size }
	{}

	constexpr size_type size() const {
		return size_;
	}

	constexpr iterator begin() const {
		return { values_ };
	}

	constexpr iterator end() const {
		return { values_ + size_ };
	}

	constexpr clear_value_type** data() const {
		return values_;
	}

	constexpr clear_value_type& operator [] (size_type index) {
		return *(begin() + index);
	}

	constexpr const clear_value_type& operator [] (size_type index) const {
		return *(begin() + index);
	}

};