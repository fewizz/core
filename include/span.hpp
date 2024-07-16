#pragma once

#include "./__type/is_reference.hpp"
#include "./__type/remove_reference.hpp"
#include "./__type/is_trivial.hpp"
#include "./__range/size.hpp"
#include "./__range/contiguous.hpp"
#include "./__range/extensions.hpp"
#include "./__range/borrowed.hpp"

template<
	typename Type,
	typename SizeType = nuint,
	typename IndexType = SizeType
>
requires(!type_is_reference<Type>)
struct span :
	borrowed_range_mark<true>,
	range_element_index_type_mark<IndexType>,
	range_extensions<span<Type, SizeType, IndexType>>
{
protected:
	Type* ptr_ = nullptr;
	SizeType size_{};
public:

	constexpr span() {};
	constexpr ~span() {
		ptr_ = nullptr;
		size_ = SizeType{};
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
	constexpr Type* sentinel() const {
		return ptr_ + (uint_of_size_of<IndexType>) size_;
	}

	constexpr Type& operator [] (IndexType index) const {
		return ptr_[(uint_of_size_of<IndexType>) index];
	}

	template<typename CastType>
	requires(
		trivial<Type> && trivial<CastType> &&
		alignof(Type) == alignof(CastType) &&
		sizeof(Type) == sizeof(CastType)
	)
	constexpr span<CastType, SizeType, IndexType> casted() const {
		return { (CastType*) ptr_, size_ };
	}

	template<typename CastType>
	requires(
		type_is_reference<CastType> &&
		alignof(Type) == alignof(void*) &&
		sizeof(Type) == sizeof(void*)
	)
	constexpr span<CastType, SizeType, IndexType> casted() const {
		return { (remove_reference<CastType>**) ptr_, size_ };
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