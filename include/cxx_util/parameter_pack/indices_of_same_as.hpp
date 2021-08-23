#pragma once

#include "indices_of.hpp"
#include "is.hpp"

namespace u {

template<typename T, typename... Ts>
using indices_of_same_as
	= u::indices_of<is<T>::template same_as, Ts...>;

} // u