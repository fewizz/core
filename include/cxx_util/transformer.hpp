#pragma once

#include <string>
#include <filesystem>
#include <type_traits>
#include <concepts>
#include "iterator.hpp"
#include "object.hpp"

namespace u {

template<typename C>
concept transformer = requires(C& c) {
	typename C::input_type;
	typename C::output_type;

	{ c(std::declval<typename C::input_type>() ) }
		-> std::same_as<typename C::output_type>;
};

template<typename T>
struct bytes_to_object_transformer {
	using input_type = std::byte;
	using output_type = T;
	using size_type = std::size_t;
	static constexpr size_type size = sizeof(T);

	template<class It> requires(
		u::iterator_of_bytes<std::remove_reference_t<It>>
	)
	output_type operator () (It&& b) const {
		return u::read_object<T>(std::forward<It>(b));
	}
};

static_assert(u::transformer<bytes_to_object_transformer<int>>);

template<class T>
struct object_to_bytes_transformer {
	using input_type = T;
	using output_type = std::byte;

	template<class It> requires(
		u::iterator_of_bytes<std::remove_reference_t<It>>
	)
	void operator() (const T& t, It&& out) {
		u::write_object(&t, std::forward<It>(out));
	}
};

static_assert(
	u::one_to_range_transformer<object_to_bytes_transformer<int>>
);

}