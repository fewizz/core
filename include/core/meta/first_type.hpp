#pragma once

#include "type.hpp"

template<typename... Types>
struct first_type_t;

template<typename FirstType, typename... TailTypes>
struct first_type_t<FirstType, TailTypes...> : type_of<FirstType> {};

template<typename... Types>
using first_type = typename first_type_t<Types...>::type;