#pragma once

#include <type_traits>

namespace u { namespace c {

template<class T>
concept trivial = std::is_trivial_v<T>;

template<class T>
concept object = std::is_object_v<T>;

}}