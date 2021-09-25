#pragma once

#include "type.hpp"

template<typename T>
struct remove_reference_t : type_of<T> {};

template<typename T>
struct remove_reference_t<T&> : type_of<T> {};

template<typename T>
struct remove_reference_t<T&&> : type_of<T> {};

template<typename T>
using remove_reference = typename remove_reference_t<T>::type;