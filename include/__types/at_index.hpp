#pragma once

#include "./of.hpp"
#include "../__integer/integer.hpp"

template<nuint Index, typename... Types>
using type_at_index = __type_pack_element<Index, Types...>;