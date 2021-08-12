#pragma once

#include <type_traits>
#include <string>

namespace u {

template<typename T>
concept null_terminated_string =
	std::is_same_v<std::decay_t<T>, const char*>
	||
	std::is_same_v<std::remove_cv_t<T>, std::string>;

template<null_terminated_string Str>
const char* data(Str& str) {
	if constexpr(std::is_same_v<std::decay_t<Str>, const char*>) {
		return str;
	}
	else if constexpr(std::is_same_v<std::remove_cv_t<Str>, std::string>) {
		return str.c_str();
	}
}

}