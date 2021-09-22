#pragma once

#include "are_same.hpp"
#include "../value.hpp"

template<typename... Types>
using are_types_same_predicate = value::of<are_types_same<Types...>>;