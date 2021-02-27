#pragma once

#include "mem_address.hpp"
#include <bits/c++config.h>
#include <cstring>
#include <memory>
#include <type_traits>

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
};

template<class T> requires std::is_object_v<T>
class obj_representation<T>
: public obj_representation_base<obj_representation<T>, T, std::byte> {

	std::byte m_obj_representation[sizeof(T)];
public:
	constexpr obj_representation(T obj) {
		std::memcpy(m_obj_representation, &obj, sizeof(T));
	}

	u::mem_address begin() const {
		return { m_obj_representation };
	}
};

template<class T> requires ( std::is_lvalue_reference_v<T> )
class obj_representation<T>
: public obj_representation_base< obj_representation<T>, T, std::byte&> {

	u::mem_address m_begin;
public:
	obj_representation(T& obj)
	: m_begin{ std::addressof(obj) } {}

	auto begin() const {
		return m_begin;
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