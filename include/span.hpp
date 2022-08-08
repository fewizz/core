#pragma once

#include "./integer.hpp"
#include "./type/is_reference.hpp"
#include "./type/remove_reference.hpp"
#include "./c_string.hpp"

template<typename Type, unsigned_integer SizeType = nuint>
struct span {
	using size_type = SizeType;

protected:
	Type* ptr_;
	size_type size_;
public:

	constexpr span() = default;

	constexpr span(Type* ptr, size_type size) : ptr_{ ptr }, size_{ size } {}
	constexpr span(size_type size, Type* ptr) : ptr_{ ptr }, size_{ size } {}

	template<nuint Size>
	constexpr span(Type (&array)[Size]) :
		ptr_{ array }, size_{ Size }
	{}

	constexpr size_type size() const { return size_; }

	constexpr const Type* iterator() const { return ptr_; }
	constexpr       Type* iterator()       { return ptr_; }

	constexpr const Type* sentinel() const { return ptr_ + size_; }
	constexpr       Type* sentinel()       { return ptr_ + size_; }

	constexpr       Type& operator [] (size_type index)       {
		return ptr_[index];
	}
	constexpr const Type& operator [] (size_type index) const {
		return ptr_[index];
	}

	constexpr       Type* elements_ptr()       { return ptr_; }
	constexpr const Type* elements_ptr() const { return ptr_; }

	template<typename CastType>
	constexpr span<CastType> cast() {
		return span<CastType>{ (CastType*) ptr_, size_ };
	}

	constexpr span shrink(size_type size) { return span{ ptr_, size }; }

	constexpr c_string<c_string_type::known_size> as_c_string() const {
		return { elements_ptr(), size() };
	}

};

template<typename Type>
span(Type*) -> span<Type>;

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

	constexpr       Type& operator [] (size_type index)       {
		return *(ptr_ + index);
	}
	constexpr const Type& operator [] (size_type index) const {
		return *(ptr_ + index);
	}

};