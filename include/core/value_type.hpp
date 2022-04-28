#pragma once

#include "element_type.hpp"

template<typename Type>
using value_type = decay<element_type<Type>>;