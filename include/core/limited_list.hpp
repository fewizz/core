#pragma once

#include "integer.hpp"
#include "exchange.hpp"
#include "placement_new.hpp"
#include "forward.hpp"
#include "meta/type/is_reference.hpp"
#include "meta/type/remove_pointer.hpp"
#include "meta/type/remove_const.hpp"
#include "meta/type/is_move_constructible.hpp"

template<typename ValueType, typename SizeType, typename Allocator>
struct limited_list {
	using value_type = ValueType;
	using element_type = value_type&;
	using size_type = SizeType;

protected:
	ValueType* ptr_ = nullptr;
	SizeType size_ = 0;
	SizeType capacity_ = 0;
	Allocator allocator_{};
public:

	constexpr limited_list() = default;

	constexpr limited_list(
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

	constexpr limited_list(limited_list&& other) :
		ptr_{ exchange(other.ptr_, nullptr) },
		size_{ exchange(other.size_, 0) },
		capacity_{ exchange(other.capacity_, 0) },
		allocator_{ move(other.allocator_) }
	{}

	constexpr limited_list(const limited_list&) = delete;

	constexpr limited_list& operator = (limited_list&& other) {
		clear();
		allocator_.deallocate((uint8*) ptr_, capacity_);
		ptr_ = nullptr;
		capacity_ = 0;

		ptr_ = exchange(other.ptr_, nullptr);
		size_ = exchange(other.size_, 0);
		capacity_ = exchange(other.capacity_, 0);
		allocator_ = move(other.allocator_);
		return *this;
	}

	constexpr limited_list& operator = (const limited_list&) = delete;

	constexpr ~limited_list() {
		clear();
		allocator_.deallocate((uint8*) ptr_, capacity_);
		ptr_ = nullptr;
		capacity_ = 0;
	}

	constexpr size_type index_of(const ValueType& v) const {
		return &v - begin();
	}

	constexpr size_type size() const { return size_; }
	constexpr size_type capacity() const { return capacity_; }

	constexpr value_type* data() { return ptr_; }
	constexpr const value_type* data() const { return ptr_; }

	constexpr value_type* begin() { return data(); }
	constexpr const value_type* begin() const { return data(); }

	constexpr value_type* end() { return begin() + size(); }
	constexpr const value_type* end() const { return begin() + size(); }

	auto& operator [] (size_type index) { return begin()[index]; }
	const auto& operator [] (size_type index) const { return begin()[index]; }

	template<typename... Args>
	value_type& emplace_back(Args&&... args) {
		new (ptr_ + size_) value_type{ forward<Args>(args)... };
		value_type& v = (*this)[size_];
		++size_;
		return v;
	}

	const ValueType& back() const { return begin()[size() - 1]; }
	auto& back() { return begin()[size() - 1]; }

	ValueType pop_back() requires move_constructible<ValueType> {
		ValueType popped{ move(back()) };
		data()[--size_].~value_type();
		return move(popped);
	}

	void pop_back() requires (!move_constructible<ValueType>) {
		data()[--size_].~value_type();
	}

	void clear() {
		while(size_ > 0) {
			pop_back();
		}
	}

};

template<typename ValueType>
class reference_limited_list_iterator {
	ValueType* ptr_;
public:

	reference_limited_list_iterator(ValueType* ptr) :
		ptr_{ ptr }
	{}

	remove_pointer<remove_const<ValueType>>& operator * () {
		return **ptr_;
	}

	const remove_pointer<remove_const<ValueType>>& operator * () const {
		return **ptr_;
	}

	auto& operator ++ () {
		++ptr_;
		return *this;
	}

	auto& operator += (nuint n) {
		ptr_ += n;
		return *this;
	}

	bool operator == (reference_limited_list_iterator it) const {
		return ptr_ == it.ptr_;
	}

	bool operator != (reference_limited_list_iterator it) const {
		return ptr_ != it.ptr_;
	}
};

template<typename RawValueType, typename SizeType, typename Allocator>
class limited_list<RawValueType&, SizeType, Allocator> :
	limited_list<remove_reference<RawValueType>*, SizeType, Allocator>
{
	using base_type = limited_list<
		remove_reference<RawValueType>*, SizeType, Allocator
	>;
	using raw_value_type = remove_reference<RawValueType>;
	using size_type = SizeType;
	using iterator_type = reference_limited_list_iterator<raw_value_type*>;
	using const_iterator_type =
		reference_limited_list_iterator<raw_value_type* const>;
public:

	using base_type::base_type;

	using base_type::size;
	using base_type::capacity;
	using base_type::pop_back;

	constexpr size_type index_of(const raw_value_type& v) const {
		size_type index{};
		for(raw_value_type* const ref : (const base_type&) *this) {
			if(ref == &v) {
				return index;
			}
			++index;
		}
		__builtin_unreachable();
	}

	constexpr size_type index_of(const raw_value_type& v) {
		size_type index{};
		for(raw_value_type* ref : (base_type&) *this) {
			if(ref == &v) {
				return index;
			}
			++index;
		}
		__builtin_unreachable();
	}

	constexpr iterator_type begin() {
		return { base_type::data() };
	}
	constexpr const_iterator_type begin() const {
		return { base_type::data() };
	}

	constexpr iterator_type end() {
		return begin() += size();
	}
	constexpr const_iterator_type end() const {
		return begin() += size();
	}

	raw_value_type& operator [] (size_type index) { return *(begin() += index); }
	const raw_value_type& operator [] (size_type index) const {
		return *(begin() += index);
	}

	template<typename... Args>
	raw_value_type& emplace_back(raw_value_type& v) {
		base_type::emplace_back(&v);
		return v;
	}

	const raw_value_type& back() const { return *this[size() - 1]; }
	raw_value_type& back() { return *this[size() - 1]; }

	raw_value_type& pop_back() {
		return *base_type::pop_back();
	}

};