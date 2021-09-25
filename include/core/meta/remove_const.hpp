#pragma once

#include "type.hpp"

template<typename T>
struct remove_const_t : type_of<T>{};

template<typename T>
struct remove_const_t<const T> : type_of<T>{};

template<typename T>
using remove_const = typename remove_const_t<T>::type;