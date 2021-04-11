#pragma once

#include <bits/c++config.h>
#include <string>
#include <filesystem>
#include <type_traits>
#include <concepts>
#include "iterator.hpp"
#include "object.hpp"

namespace u {

struct one_to_one_converter_tag {};
struct one_to_range_converter_tag {};
struct range_to_one_converter_tag {};

enum class convert_error {
	invalid_input,
	unexpected_src_end
};

template<class C>
concept one_to_one_converter = requires(C& c) {
	typename C::input_type;
	typename C::output_type;

	{ c.convert(std::declval<typename C::input_type>() ) }
		-> std::same_as<typename C::output_type>;
};

template<class C>
concept one_to_range_converter = requires(C& c) {
	typename C::input_type;
	typename C::output_type;

	{ 
		c.convert(
			std::declval<typename C::input_type>(),
			std::declval<std::add_pointer_t<typename C::output_type>>()
		)
	} -> std::same_as<void>;
};

template<class C>
concept range_to_one_converter =
	requires(C& c) {
		typename C::input_type;
		typename C::output_type;
		typename C::size_type;
	} && 
	requires(C& c, std::add_pointer_t<typename C::input_type> i) {
		{ c.convert(i) } -> std::same_as<typename C::output_type>;
	};

template<class C>
concept dynamic_size_range_to_one_converter =
	range_to_one_converter<C> &&
	requires(C& c, std::add_pointer_t<typename C::input_type> i) {
		{ c.size(i) } -> std::same_as<typename C::size_type>;
	};

template<class C>
concept fixed_size_range_to_one_converter =
	range_to_one_converter<C> &&
	requires(C& c) {
		{ C::size } -> std::same_as<const typename C::size_type>;
	};

template<class T>
struct bytes_to_object_converter {
	using input_type = std::byte;
	using output_type = T;
	using size_type = std::size_t;
	static constexpr size_type size = sizeof(T);

	template<class It> requires(
		u::iterator_of_bytes<std::remove_reference_t<It>>
	)
	output_type convert(It b) const {
		return u::read_object<T>(std::forward<It>(b));
	}
};

static_assert(
	u::fixed_size_range_to_one_converter<bytes_to_object_converter<int>>
);

template<class T>
struct object_to_bytes_converter {
	using input_type = T;
	using output_type = std::byte;

	template<class It> requires(
		u::iterator_of_bytes<std::remove_reference_t<It>>
	)
	void convert(const T& t, It&& out) {
		u::write_object(&t, std::forward<It>(out));
	}
};

static_assert(
	u::one_to_range_converter<object_to_bytes_converter<int>>
);

}