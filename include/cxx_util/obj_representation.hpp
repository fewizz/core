#pragma once

#include "mem_address.hpp"
#include <cstring>
#include <memory>
#include <type_traits>
#include <new>
#include "iterator.hpp"

namespace u {

template<class T>
class obj_representation;

template<class D, class T, class ValueReturn>
class obj_representation_base {
	auto begin() const {
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

	ValueReturn operator [] (size_type n) {
		return *(begin() + n);
	}

	std::array<std::byte, sizeof(T)> to_array() {
		std::array<std::byte, sizeof(T)> rep;
		std::copy(begin(), end(), rep.begin());
		return rep;
	}

	T create()
	requires(std::copy_constructible<T>) {
		alignas(T) std::byte rep[sizeof(T)];
		std::copy(begin(), end(), rep);
		T* t_ptr = new (rep) T;
		return { *t_ptr };
	}
};

template<class T> requires std::is_object_v<T>
class obj_representation<T>
: public obj_representation_base<
	obj_representation<T>,
	T,
	std::byte
> {

	alignas(T) std::byte m_obj_representation[sizeof(T)];
public:
	constexpr obj_representation(T obj) {
		std::memcpy(m_obj_representation, &obj, sizeof(T));
	}

	obj_representation(
		u::iterator_of_bytes auto b
	) {
		auto e = b;
		std::advance(e, sizeof(T));
		std::copy(b, e, m_obj_representation);
	}

	u::mem_address begin() const {
		return { m_obj_representation };
	}
};

template<class T> requires ( std::is_lvalue_reference_v<T> )
class obj_representation<T>
: public obj_representation_base<
	obj_representation<T>,
	std::remove_reference_t<T>,
	std::byte&
> {

	u::mem_address m_address;
public:
	obj_representation(T& obj)
	: m_address{ std::addressof(obj) } {}

	auto begin() const {
		return m_address;
	}
};

template<class T>
obj_representation(T& t) -> obj_representation<T&>;

auto make_obj_representation_copy(const auto& v) {
	return obj_representation<
		std::remove_cvref_t<decltype(v)>
	>{ v };
}

}