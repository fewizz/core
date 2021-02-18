#pragma once

#include <string>
#include <filesystem>
#include <type_traits>
//#include "mb/string.hpp"
//#include "mb/string_view.hpp"
#include <concepts>

namespace util {

/*template<class To, enc::encoding E, class CharT>
requires(std::is_same_v<std::decay_t<To>, std::basic_string<CharT>>)
auto convert_to(const mb::basic_string<E>& str) {
	return str.template to_string<CharT>();
}

template<class To, enc::encoding E>
requires(std::is_same_v<To, bool>)
bool convert_to(const mb::basic_string_view<E>& str) {
	return !str.empty() && str[0] == '1';
}

template<class To, enc::encoding E>
requires(std::is_same_v<To, std::filesystem::path>)
std::filesystem::path convert_to(const mb::basic_string_view<E>& str) {
	return { str.template to_string<typename E::char_type>() };
}

template<class To, enc::encoding E>
requires(std::is_same_v<To, mb::basic_string<E>>)
mb::basic_string<E> convert_to(const mb::basic_string_view<E>& str) {
	return { str.template to_string<E>() };
}

template<class To, enc::encoding E>
requires(std::is_same_v<To, std::basic_string<typename E::char_type>>)
std::basic_string<typename E::char_type> convert_to(const mb::basic_string_view<E>& str) {
	return { str.template to_string<typename E::char_type>() };
}

template<class To, class From>
requires(
	not std::is_same_v<To, std::filesystem::path> // Error occurs while substituting
	&& 
	std::is_constructible_v<To, From>
)
To convert_to(From&& from) {
	return To{ std::forward<From>(from) };
}*/

template<class To, class From>
concept convertible_from = not std::is_same_v<void, decltype(convert_to<To>(std::declval<From>()))>;

template<class From, class To>
concept convertible_to = convertible_from<To, From>;

template<class To, class From>
struct is_convertible_to : std::false_type {};

template<class To, convertible_to<To> From>
struct is_convertible_to<To, From> : std::true_type {};

template<class To, class From>
static constexpr bool is_convertible_to_v = is_convertible_to<To, From>::value;

}