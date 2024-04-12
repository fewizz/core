#pragma once

#include "./__type/is_reference.hpp"
#include "./__type/remove_reference.hpp"
#include "./__type/is_trivial.hpp"
#include "./__range/size.hpp"
#include "./__range/contiguous.hpp"
#include "./__range/extensions.hpp"

template<typename Type, unsigned_integer SizeType = nuint>
requires(!type_is_reference<Type>)
struct span : range_extensions<span<Type, SizeType>> {
protected:
	Type* ptr_ = nullptr;
	SizeType size_ = 0;
public:

	static constexpr bool is_borrowed_range = true;

	constexpr span() {};
	constexpr ~span() {
		ptr_ = nullptr;
		size_ = 0;
	}

	constexpr span(Type* ptr) : ptr_{ ptr }, size_{ 1 } {}
	constexpr span(Type* ptr, SizeType size) : ptr_{ ptr }, size_{ size } {}

	template<nuint Size>
	constexpr span(Type (&array)[Size]) :
		ptr_{ array }, size_{ Size }
	{}

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

	template<typename CastType, typename CastSizeType = nuint>
	requires(
		trivial<Type> && trivial<CastType> &&
		alignof(Type) == alignof(CastType) &&
		sizeof(Type) == sizeof(CastType)
	)
	constexpr span<CastType, CastSizeType> cast() const {
		return { (CastType*) ptr_, (CastSizeType) size_ };
	}

	template<typename CastType, typename CastSizeType = nuint>
	requires(
		type_is_reference<CastType> &&
		alignof(Type) == alignof(void*) &&
		sizeof(Type) == sizeof(void*)
	)
	constexpr span<CastType, CastSizeType> cast() const {
		return { (remove_reference<CastType>**) ptr_, (CastSizeType) size_ };
	}

	constexpr span shrink(SizeType size) const {
		return { ptr_, size };
	}

	constexpr span<const Type> const_elements() const {
		return { ptr_, size_ };
	}

};

template<typename Type>
span(Type*) -> span<Type>;