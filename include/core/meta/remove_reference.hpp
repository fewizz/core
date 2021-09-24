#pragma once

#include "type.hpp"

template<typename T>
struct remove_reference_type : type_of<T> {};

template<typename T>
struct remove_reference_type<T&> : type_of<T> {};

template<typename T>
struct remove_reference_type<T&&> : type_of<T> {};

template<typename T>
using remove_reference = typename remove_reference_type<T>::type;