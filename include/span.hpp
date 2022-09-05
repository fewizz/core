#pragma once

#include "./integer.hpp"
#include "./__type/is_reference.hpp"
#include "./__type/remove_reference.hpp"
#include "./__range/size.hpp"
#include "./__range/basic.hpp"
#include "./__range/element_type.hpp"

template<typename Type, unsigned_integer SizeType = nuint>
struct span {
protected:
	Type* ptr_;
	SizeType size_;
public:

	constexpr span() = default;

	constexpr span(Type* ptr, SizeType size) : ptr_{ ptr }, size_{ size } {}
	constexpr span(SizeType size, Type* ptr) : ptr_{ ptr }, size_{ size } {}

	template<nuint Size>
	constexpr span(Type (&array)[Size]) :
		ptr_{ array }, size_{ Size }
	{}

	// TODO contiguous range, range_size_type
	template<sized_range Range>
	constexpr span(Range&& range) :
		ptr_{ range.elements_ptr() }, size_{ (SizeType) range_size(range) }
	{}

	constexpr SizeType size() const { return size_; }

	constexpr const Type* iterator() const { return ptr_; }
	constexpr       Type* iterator()       { return ptr_; }

	constexpr const Type* sentinel() const { return ptr_ + size_; }
	constexpr       Type* sentinel()       { return ptr_ + size_; }

	constexpr       Type& operator [] (SizeType index)       {
		return ptr_[index];
	}
	constexpr const Type& operator [] (SizeType index) const {
		return ptr_[index];
	}

	constexpr       Type* elements_ptr()       { return ptr_; }
	constexpr const Type* elements_ptr() const { return ptr_; }

	template<typename CastType>
	constexpr span<CastType> cast() {
		return span<CastType> {
			(CastType*) ptr_,
			size_ * sizeof(Type) / sizeof(CastType)
		};
	}

	constexpr span shrink(SizeType size) { return span{ ptr_, size }; }

};

template<typename Type>
span(Type*) -> span<Type>;

template<basic_range Range>
span(Range&&) -> span<remove_reference<range_element_type<Range>>>;

template<typename Type, unsigned_integer SizeType>
struct span<Type&, SizeType> {
	using size_type = SizeType;
private:
	Type**    ptr_;
	size_type size_;
public:
	
	class it {
		Type** ptr_;
	public:

		constexpr it(Type** ptr) : ptr_{ ptr } {}

		constexpr Type& operator * () const { return **ptr_; }

		constexpr it& operator ++ () { ++ptr_; return *this; }

		constexpr it& operator += (size_type n) {
			ptr_ += n; return *this;
		}
	
		constexpr it operator + (size_type n) const {
			return it{ *this } += n;
		}
		constexpr it operator + (size_type n)       {
			return it{ *this } += n;
		}
	
		constexpr bool operator == (const it other) const {
			return ptr_ == other.ptr_;
		}
	};

	constexpr span(Type** ptr, size_type size) : ptr_{ ptr }, size_{ size } {}
	constexpr span(size_type size, Type** ptr) : ptr_{ ptr }, size_{ size } {}

	constexpr size_type size() const { return size_; }

	constexpr it iterator() const { return { ptr_ }; }
	constexpr it iterator()       { return { ptr_ }; }

	constexpr it sentinel() const { return { ptr_ + size_ }; }
	constexpr it sentinel()       { return { ptr_ + size_ }; }

	constexpr const Type** elements_ptr() const { return ptr_; }
	constexpr       Type** elements_ptr()       { return ptr_; }

	constexpr const Type& operator [] (size_type index) const {
		return **(ptr_ + index);
	}
	constexpr       Type& operator [] (size_type index)       {
		return **(ptr_ + index);
	}

};