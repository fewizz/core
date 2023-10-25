#pragma once

#include "./type.hpp"
#include "./__type/remove_reference.hpp"
#include "../__type/is_same_as.hpp"
#include "../integer.hpp"
#include "../__range/basic.hpp"

/* it's *sentinel() == '\0' */
template<c_string_type Sized, typename Type = char, typename SizeType = nuint>
struct c_string;

template<typename Type, typename SizeType = nuint>
using c_string_of_unknown_size
	= c_string<c_string_type::unknown_size, Type, SizeType>;
template<typename Type, typename SizeType = nuint>
using c_string_of_known_size
	= c_string<c_string_type::known_size, Type, SizeType>;

template<typename Type, typename CharType = char>
concept any_c_string =
	basic_range<Type> && (
	same_as<remove_reference<Type>, c_string_of_known_size<CharType>> ||
	same_as<remove_reference<Type>, c_string_of_unknown_size<CharType>>);