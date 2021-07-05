#pragma once

#include "mem_address.hpp"
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>
#include <new>
#include "iterator/iterator.hpp"
#include "concepts.hpp"
#include "object/operations.hpp"

namespace u {

namespace internal {

template<class Derived, class T, class ValueReturn>
class object_representation_base {
	constexpr const T* begin() const {
		return ((Derived*)this)->begin();
	}

	constexpr T* begin() {
		return ((Derived*)this)->begin();
	}

public:
	using size_type = std::size_t;
	using iterator = u::mem_address;

	constexpr size_type size() const {
		return sizeof(T);
	}

	constexpr iterator end() const {
		return begin() + size();
	}

	constexpr ValueReturn front() const {
		return *begin();
	}

	constexpr ValueReturn back() const {
		return *(end() - 1);
	}

	constexpr ValueReturn& operator [] (size_type n) {
		return *(begin() + n);
	}

	constexpr std::array<std::byte, sizeof(T)> to_array() {
		std::array<std::byte, sizeof(T)> rep;
		std::ranges::copy(*this, rep.begin());
		return rep;
	}

	constexpr T create() {
		return read_object<T>(begin());
	}
};

}

template<class T> requires std::is_object_v<T>
class object_representation_copy
: public internal::object_representation_base<
	object_representation_copy<T>,
	T,
	std::byte
> {

	alignas(T) std::byte m_obj_representation[sizeof(T)];

public:

	constexpr object_representation_copy(const T& obj) {
		std::memcpy(m_obj_representation, &obj, sizeof(T));
	}

	constexpr const std::byte* begin() const {
		return m_obj_representation;
	}

	constexpr std::byte* begin() {
		return { m_obj_representation };
	}
};

template<class T> requires std::is_object_v<T>
class object_representation_reference
: public internal::object_representation_base<
	object_representation_reference<T>,
	T,
	std::byte&
> {

	T& m_obj;

public:

	constexpr object_representation_reference(T& obj)
	: m_obj { obj } {}

	constexpr const T& begin() const {
		return &m_obj;
	}

	constexpr T& begin() {
		return &m_obj;
	}
};

}