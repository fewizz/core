#pragma once

#include <type_traits>
#include <cstddef>
#include <iterator>

namespace u {

template<typename T>
concept trivial = std::is_trivial_v<T>;

template<typename T>
concept object = std::is_object_v<T>;

template<typename T>
constexpr bool is_atom =
	std::is_same_v<T, std::byte>
	|| std::is_same_v<T, unsigned char>;

template<typename T>
concept atom = is_atom<T>;

template<typename T>
concept atom_input_or_output =
	std::input_or_output_iterator<T>
	&& is_atom<std::iter_value_t<T>>;

template<typename T>
concept atom_input_iterator =
	std::input_iterator<T>
	&& is_atom<std::iter_value_t<T>>;

template<typename T>
concept atom_output_iterator =
	std::output_iterator<T, std::byte>
	|| std::output_iterator<T, unsigned char>;

}