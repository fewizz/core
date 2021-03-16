#pragma once

#include "mem_address.hpp"
#include <bits/c++config.h>
#include <cstring>
#include <iterator>
#include <memory>
#include <type_traits>
#include <new>
#include "iterator.hpp"

namespace u {

template<
	class T,
	std::endian E = std::endian::native,
	class It
>
requires (
	std::is_trivial_v<T>
	&&
	std::is_default_constructible_v<T>
	&&
	u::iterator_of_bytes<std::remove_reference_t<It>>
)
T read_object(It&& it) {
	alignas(T) std::byte representation[sizeof(T)];

	for(std::size_t i = 0; i < sizeof(T); i++) {
		representation[
			E == std::endian::native ? i : sizeof(T) - i - 1
		] = *it++;
	}

	T t;
	std::memcpy(&t, representation, sizeof(T));
	return t;
}

template<
	class T,
	std::endian E = std::endian::native,
	class It
>
requires (
	std::is_trivial_v<T>
	&&
	std::is_default_constructible_v<T>
	&&
	u::iterator_of_bytes<std::remove_reference_t<It>>
)
void write_object(const T& t, It&& it) {
	for(std::size_t i = 0; i < sizeof(T); i++) {
		*it++ = ( (std::byte*) &t )[
			E == std::endian::native ? i : sizeof(T) - i - 1
		];
	}
}

namespace internal {

template<class D, class T, class ValueReturn>
class obj_representation_base {
	u::mem_address begin() const {
		return ((D*)this)->begin();
	}

public:

	using size_type = std::size_t;
	using iterator = u::mem_address;

	constexpr size_type size() const {
		return sizeof(T);
	}

	iterator end() const {
		return begin() + size();
	}

	ValueReturn front() const {
		return *begin();
	}

	ValueReturn back() const {
		return *(end() - 1);
	}

	ValueReturn& operator [] (size_type n) {
		return *(begin() + n);
	}

	std::array<std::byte, sizeof(T)> to_array() {
		std::array<std::byte, sizeof(T)> rep;
		std::copy(begin(), end(), rep.begin());
		return rep;
	}

	T create() {
		return read_object<T>(begin());
	}
};

}

template<class T> requires std::is_object_v<T>
class obj_representation_copy
: public internal::obj_representation_base<
	obj_representation_copy<T>,
	T,
	std::byte
> {

	alignas(T) std::byte m_obj_representation[sizeof(T)];

public:

	obj_representation_copy(const T& obj) {
		std::memcpy(m_obj_representation, &obj, sizeof(T));
	}

	u::mem_address begin() const {
		return { m_obj_representation };
	}
};

template<class T> requires std::is_object_v<T>
class obj_representation_reference
: public internal::obj_representation_base<
	obj_representation_reference<T>,
	T,
	std::byte&
> {

	T& m_obj;

public:

	obj_representation_reference(T& obj)
	: m_obj { obj } {}

	auto begin() const {
		return u::mem_address{ std::addressof(m_obj) };
	}
};

}