#pragma once

#include "./exchange.hpp"
#include "./placement_new.hpp"
#include "./forward.hpp"
#include "./memory_span.hpp"
#include "./type/is_reference.hpp"
#include "./type/remove_pointer.hpp"
#include "./type/remove_const.hpp"
#include "./type/is_move_constructible.hpp"

template<typename ValueType, typename SizeType = nuint>
struct memory_list {
protected:
	ValueType* ptr_      = nullptr;
	SizeType   capacity_bytes_ = 0;
	SizeType   size_     = 0;
public:

	constexpr memory_list() = default;

	constexpr memory_list(memory_span memory_span) :
		ptr_     { (ValueType*) memory_span.elements_ptr() },
		capacity_bytes_{ (SizeType) memory_span.size() }
	{}

	constexpr memory_list(memory_list&& other) :
		ptr_            { exchange(other.ptr_,      nullptr) },
		capacity_bytes_ { exchange(other.capacity_bytes_, 0)       },
		size_           { exchange(other.size_,     0)       }
	{}

	constexpr memory_list(const memory_list&) = delete;

	constexpr memory_list& operator = (memory_list&& other) {
		clear();
		ptr_             = exchange(other.ptr_,       nullptr);
		capacity_bytes_  = exchange(other.capacity_bytes_ , 0);
		size_            = exchange(other.size_,            0);
		return *this;
	}

	constexpr memory_list& operator = (const memory_list&) = delete;

	constexpr ~memory_list() {
		clear();
	}

	constexpr ::memory_span memory_span() const {
		return { (uint8*) ptr_, capacity_bytes_ };
	}

	constexpr SizeType size()     const { return size_;     }
	constexpr SizeType capacity() const {
		return capacity_bytes_ / sizeof(ValueType);
	}

	constexpr const ValueType* elements_ptr() const { return ptr_; }
	constexpr       ValueType* elements_ptr()       { return ptr_; }

	constexpr const ValueType* iterator() const { return ptr_; }
	constexpr       ValueType* iterator()       { return ptr_; }

	constexpr const ValueType* sentinel() const { return ptr_ + size_; }
	constexpr       ValueType* sentinel()       { return ptr_ + size_; }

	const ValueType& operator [] (SizeType index) const { return ptr_[index]; }
	      ValueType& operator [] (SizeType index)       { return ptr_[index]; }

	template<typename... Args>
	ValueType& emplace_back(Args&&... args) {
		ValueType* v = new (ptr_ + size_)
			ValueType{ forward<Args>(args)... };
		++size_;
		return *v;
	}

	template<typename... Args>
	void emplace_at(nuint index, Args&&... args) {
		(ptr_ + index)->~ValueType();
		new (ptr_ + index)
			ValueType{ forward<Args>(args)... };
	}

	ValueType&  back() const &  { return ptr_[size() - 1]; }
	ValueType&  back()       &  { return ptr_[size() - 1]; }

	ValueType pop_back() requires ( move_constructible<ValueType>) {
		ValueType popped{ move(back()) };
		--size_;
		ptr_[size_].~ValueType();
		return popped;
	}
	void      pop_back() requires (!move_constructible<ValueType>) {
		--size_;
		ptr_[size_].~ValueType();
	}

	auto pop_back(SizeType count) {
		while (count > 0) {
			pop_back();
			--count;
		}
	}

	void fill(const ValueType& element) {
		while(size_ < capacity()) {
			emplace_back(element);
		}
	}

	void clear() {
		pop_back(size_);
	}

};

template<typename ValueType, typename SizeType>
class reference_memory_list_iterator {
	ValueType** ptr_;
public:

	reference_memory_list_iterator(ValueType** ptr) :
		ptr_{ ptr }
	{}

	const ValueType& operator * () const { return **ptr_; }
	      ValueType& operator * ()       { return **ptr_; }

	auto& operator ++ () { ++ptr_; return *this; }
	auto& operator -- () { --ptr_; return *this; }

	auto& operator += (SizeType n) { ptr_ += n; return *this; }
	auto& operator -= (SizeType n) { ptr_ -= n; return *this; }

	auto operator + (SizeType n) const {
		reference_memory_list_iterator cpy = *this;
		return cpy += n;
	}

	auto operator - (SizeType n) const {
		reference_memory_list_iterator cpy = *this;
		return cpy -= n;
	}

	SizeType operator - (reference_memory_list_iterator other) const {
		return (SizeType) (ptr_ - other.ptr_);
	}

	bool operator == (reference_memory_list_iterator it) const {
		return ptr_ == it.ptr_;
	}

	bool operator != (reference_memory_list_iterator it) const {
		return ptr_ != it.ptr_;
	}
};

template<typename RawValueType, typename SizeType>
class memory_list<RawValueType&, SizeType> :
	memory_list<remove_reference<RawValueType>*, SizeType>
{
	using base_type = memory_list<RawValueType*, SizeType>;
	using iterator_type =
		reference_memory_list_iterator<RawValueType, SizeType>;
	using const_iterator_type =
		reference_memory_list_iterator<const RawValueType, SizeType>;

protected:
	using base_type::ptr_;
public:

	using base_type::base_type;

	using base_type::size;
	using base_type::capacity;
	using base_type::pop_back;

	constexpr const_iterator_type iterator() const { return ptr_; }
	constexpr       iterator_type iterator()       { return ptr_; }

	constexpr const_iterator_type sentinel() const { return ptr_ + size(); }
	constexpr       iterator_type sentinel()       { return ptr_ + size(); }

	const RawValueType& operator [] (SizeType index) const {
		return **(ptr_ + index);
	}
	      RawValueType& operator [] (SizeType index)       {
		return **(ptr_ + index);
	}

	void emplace_at(nuint index, RawValueType& value) {
		base_type::operator[](index) = &value;
	}

	template<typename... Args>
	RawValueType& emplace_back(RawValueType& v) {
		base_type::emplace_back(&v);
		return v;
	}

	void fill(const RawValueType& element) {
		base_type::fill(&element);
	}

	const RawValueType& back() const { return *base_type::back(); }
	      RawValueType& back()       { return *base_type::back(); }

	RawValueType& pop_back() {
		return *base_type::pop_back();
	}

};