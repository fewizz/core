#pragma once

#include "integer.hpp"
#include "exchange.hpp"
#include "placement_new.hpp"
#include "forward.hpp"

template<
	typename ValueType, typename SizeType, typename Allocator
>
class fixed_vector {
	using value_type = ValueType;
	using size_type = SizeType;

	ValueType* ptr_ = nullptr;
	SizeType size_ = 0;
	SizeType capacity_ = 0;
	Allocator allocator_{};
public:

	constexpr fixed_vector() = default;

	constexpr fixed_vector(
		size_type capacity,
		Allocator&& allocator = {}
	) :
		size_{ 0 },
		capacity_{ capacity },
		allocator_{ allocator }
	{
		if(capacity > 0) {
			ptr_ = (value_type*) allocator.allocate(
				sizeof(value_type) * capacity
			);
		}
	}

	constexpr fixed_vector(fixed_vector&& other) :
		ptr_{ exchange(other.ptr_, nullptr) },
		size_{ exchange(other.size_, 0) },
		capacity_{ exchange(other.capacity_, 0) },
		allocator_{ move(other.allocator_) }
	{}

	constexpr fixed_vector& operator = (fixed_vector&& other) {
		ptr_ = exchange(other.ptr_, nullptr);
		size_ = exchange(other.size_, 0);
		capacity_ = exchange(other.capacity_, 0);
		allocator_ = move(other.allocator_);
		return *this;
	}

	constexpr ~fixed_vector() {
		for(size_type i = size(); i > 0;) {
			data()[--i].~value_type();
		}
		allocator_.deallocate((uint8*) ptr_, capacity_);
	}

	constexpr size_type size() const { return size_; }
	constexpr size_type capacity() const { return capacity_; }

	constexpr value_type* data() { return ptr_; }
	constexpr const value_type* data() const { return ptr_; }

	constexpr value_type* begin() { return data(); }
	constexpr const value_type* begin() const { return data(); }

	constexpr value_type* end() { return begin() + size(); }
	const value_type* end() const {return begin() + size(); }

	auto& operator [] (size_type index) { return begin()[index]; }
	const auto& operator [] (size_type index) const { return begin()[index]; }

	template<typename... Args>
	void emplace_back(Args&&... args) {
		new (ptr_ + size_) value_type{ forward<Args>(args)... };
		++size_;
	}

	const auto& back() const { return begin()[size() - 1]; }
	auto& back() { return begin()[size() - 1]; }
};