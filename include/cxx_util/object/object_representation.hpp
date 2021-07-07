#pragma once

#include "concepts.hpp"
#include <array>
#include <span>
#include "operations.hpp"

namespace u {

template<u::c::object T>
struct object_representation_copy : std::array<std::byte, sizeof(T)> {
	using base_type = std::array<std::byte, sizeof(T)>;

	object_representation_copy(const T& obj) {
		u::object_to_bytes(obj, base_type::begin());
	}

	T create() const {
		return u::object_from_bytes<T>(base_type::begin());
	}
};

template<u::c::object T>
struct object_representation_reference : std::span<std::byte, sizeof(T)> {
	using base_type = std::span<std::byte, sizeof(T)>;

	object_representation_reference(const T& obj)
		: base_type { (std::byte*)&obj, sizeof(T) } {}

	T create() const {
		return u::object_from_bytes<T>(base_type::begin());
	}
};

}