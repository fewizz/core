#pragma once

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

template<class T>
struct bytes_to_object_converter {
	using input_type = std::byte;
	using output_type = T;

	template<class It> requires(
		u::iterator_of_bytes<std::remove_reference_t<It>>
	)
	output_type convert(It&& b) {
		return u::read_object<T>(std::forward<It>(b));
	}
};

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

}