#pragma once

#include <string>
#include <filesystem>
#include <type_traits>
#include <concepts>

namespace u {

template<class To, class From>
concept convertible_from =
	not std::is_same_v<
		void,
		decltype( convert_to<To>( std::declval<From>() ) )
	>;

template<class From, class To>
concept convertible_to = convertible_from<To, From>;

template<class To, class From>
struct is_convertible_to : std::false_type {};

template<class To, convertible_to<To> From>
struct is_convertible_to<To, From> : std::true_type {};

template<class To, class From>
static constexpr bool is_convertible_to_v = is_convertible_to<To, From>::value;

template<class T>
T convert(const T& t) {
	return t;
}

}