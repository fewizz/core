#pragma once

#include "./integer.hpp"
#include "./exchange.hpp"
#include "./placement_new.hpp"
#include "./forward.hpp"
#include "./memory_span.hpp"
#include "./type/is_reference.hpp"
#include "./type/remove_pointer.hpp"
#include "./type/remove_const.hpp"
#include "./type/is_move_constructible.hpp"

template<typename ValueType, typename SizeType>
struct memory_list {
protected:
	using size_type = SizeType;
	SizeType size_ = 0;
	memory_span memory_span_;
public:

	constexpr memory_list() = default;

	constexpr memory_list(memory_span memory_span) :
		size_{ 0 },
		memory_span_{ memory_span }
	{}

	constexpr memory_list(memory_list&& other) :
		size_       { exchange(other.size_, 0)                    },
		memory_span_{ exchange(other.memory_span_, memory_span{}) }
	{}

	constexpr memory_list(const memory_list&) = delete;

	constexpr memory_list& operator = (memory_list&& other) {
		clear();
		size_        = exchange(other.size_, 0);
		memory_span_ = exhange(other.memory_span, memory_span{});
		return *this;
	}

	constexpr memory_list& operator = (const memory_list&) = delete;

	constexpr ~memory_list() {
		clear();
		memory_span_ = {};
	}

	constexpr size_type size() const { return size_; }
	constexpr size_type capacity() const {
		return memory_span_.size() / sizeof(ValueType);
	}

	constexpr const ValueType* elements_ptr() const {
		return (ValueType*) memory_span_.elements_ptr();
	}
	constexpr       ValueType* elements_ptr()       {
		return (ValueType*) memory_span_.elements_ptr();
	}

	constexpr const ValueType* iterator() const { return elements_ptr(); }
	constexpr       ValueType* iterator()       { return elements_ptr(); }

	constexpr const ValueType* sentinel() const { return iterator() + size(); }
	constexpr       ValueType* sentinel()       { return iterator() + size(); }

	const ValueType& operator [] (size_type index) const {
		return elements_ptr()[index];
	}
	      ValueType& operator [] (size_type index)       {
		return elements_ptr()[index];
	}

	template<typename... Args>
	ValueType& emplace_back(Args&&... args) {
		ValueType* v = new (memory_span_.elements_ptr() + size_)
			ValueType{ forward<Args>(args)... };
		++size_;
		return v;
	}

	decltype(auto) back() const { return elements_ptr()[size() - 1]; }
	decltype(auto) back()       { return elements_ptr()[size() - 1]; }

	ValueType pop_back() requires move_constructible<ValueType> {
		ValueType popped{ move(back()) };
		elements_ptr()[--size_].~value_type();
		return popped;
	}

	void pop_back() requires (!move_constructible<ValueType>) {
		elements_ptr()[--size_].~value_type();
	}

	void fill(const ValueType& element) {
		while(size() < capacity()) {
			emplace_back(element);
		}
	}

	void clear() {
		while(size_ > 0) {
			pop_back();
		}
	}

};

template<typename ValueType>
class reference_memory_list_iterator {
	ValueType** ptr_;
public:

	reference_memory_list_iterator(ValueType** ptr) :
		ptr_{ ptr }
	{}

	const ValueType& operator * () const { return **ptr_; }
	      ValueType& operator * ()       { return **ptr_; }

	auto& operator ++ () { ++ptr_; return *this; }

	auto& operator += (nuint n) { ptr_ += n; return *this; }

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
	using base_type = memory_list<
		remove_reference<RawValueType>*, SizeType
	>;
	using size_type = SizeType;
	using iterator_type = reference_memory_list_iterator<RawValueType*>;
	using const_iterator_type =
		reference_memory_list_iterator<const RawValueType*>;
public:

	using base_type::base_type;

	using base_type::size;
	using base_type::capacity;
	using base_type::pop_back;

	constexpr const_iterator_type iterator() const {
		return base_type::elements_ptr();
	}
	constexpr       iterator_type iterator() {
		return base_type::elements_ptr();
	}

	constexpr const_iterator_type sentinel() const {
		return iterator() += size();
	}
	constexpr       iterator_type sentinel()       {
		return iterator() += size();
	}

	const RawValueType& operator [] (size_type index) const {
		return *(iterator() + index);
	}
	      RawValueType& operator [] (size_type index)       {
		return *(iterator() + index);
	}

	template<typename... Args>
	RawValueType& emplace_back(RawValueType& v) {
		base_type::emplace_back(&v);
		return v;
	}

	void fill(const RawValueType& element) {
		base_type::fill(&element);
	}

	const RawValueType& back() const { return *this[size() - 1]; }
	      RawValueType& back()       { return *this[size() - 1]; }

	RawValueType& pop_back() {
		return *base_type::pop_back();
	}

};