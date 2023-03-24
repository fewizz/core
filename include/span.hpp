#pragma once

#include "./integer.hpp"
#include "./__type/is_reference.hpp"
#include "./__type/remove_reference.hpp"
#include "./__type/is_trivial.hpp"
#include "./__range/size.hpp"
#include "./__range/contiguous.hpp"
#include "./__range/element_type.hpp"
#include "./__range/extensions.hpp"

template<typename Type, unsigned_integer SizeType = nuint>
struct span : range_extensions<span<Type, SizeType>> {
protected:
	Type* ptr_ = nullptr;
	SizeType size_ = 0;
public:

	static constexpr bool is_borrowed_range = true;

	constexpr span() {};

	constexpr span(Type* ptr) : ptr_{ ptr }, size_{ 1 } {}
	constexpr span(Type* ptr, SizeType size) : ptr_{ ptr }, size_{ size } {}

	template<nuint Size>
	constexpr span(Type (&array)[Size]) :
		ptr_{ array }, size_{ Size }
	{}

	// TODO contiguous range, range_size_type
	template<contiguous_range Range>
	constexpr span(Range&& range) :
		ptr_{ range_iterator(range) },
		size_{ static_cast<SizeType>(range_size(range)) }
	{}

	constexpr SizeType size() const { return size_; }

	constexpr Type* iterator() const { return ptr_; }
	constexpr Type* sentinel() const { return ptr_ + size_; }

	constexpr Type& operator [] (SizeType index) const {
		return ptr_[index];
	}

	template<typename CastType>
	requires(
		trivial<Type> && trivial<CastType> &&
		alignof(Type) == alignof(CastType) &&
		sizeof(Type) == sizeof(CastType)
	)
	constexpr span<CastType> cast() const {
		return { (CastType*) ptr_, size_ };
	}

	constexpr span shrink(SizeType size) const {
		return { ptr_, size };
	}

};

template<typename Type>
span(Type*) -> span<Type>;

template<contiguous_range Range>
span(Range&&) -> span<remove_reference<range_element_type<Range>>>;

template<typename Type, unsigned_integer IndexType>
struct span<Type&, IndexType> : range_extensions<span<Type&, IndexType>> {
	using index_type = IndexType;
private:
	Type**    ptr_;
	index_type size_;
public:
	
	class it {
		Type** ptr_;
	public:

		constexpr it(Type** ptr) : ptr_{ ptr } {}

		constexpr Type& operator * () const { return **ptr_; }

		constexpr it& operator ++ () { ++ptr_; return *this; }

		constexpr it& operator += (index_type n) {
			ptr_ += n; return *this;
		}
	
		constexpr it operator + (index_type n) const {
			return it{ *this } += n;
		}
		constexpr it operator + (index_type n)       {
			return it{ *this } += n;
		}
	
		constexpr bool operator == (const it other) const {
			return ptr_ == other.ptr_;
		}
	};

	constexpr span() : ptr_{ nullptr }, size_{ 0 } {}
	constexpr span(Type** ptr, index_type size) : ptr_{ ptr }, size_{ size } {}

	constexpr index_type size() const { return size_; }

	constexpr it iterator() const { return { ptr_ }; }
	constexpr it sentinel() const { return { ptr_ + size_ }; }

	constexpr Type& operator [] (index_type index) const {
		return **(ptr_ + index);
	}

};