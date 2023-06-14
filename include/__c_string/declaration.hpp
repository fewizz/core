#pragma once

#include "./type.hpp"
#include "../__type/is_same_as.hpp"

/* it's *sentinel() == '\0' */
template<c_string_type Sized, typename Type = char>
struct c_string;

template<typename Type>
using c_string_of_unknown_size = c_string<c_string_type::unknown_size, Type>;
template<typename Type>
using c_string_of_known_size   = c_string<c_string_type::known_size, Type>;

template<typename Type, typename CharType = char>
concept any_c_string =
	same_as<Type, c_string_of_known_size<CharType>> ||
	same_as<Type, c_string_of_unknown_size<CharType>>;