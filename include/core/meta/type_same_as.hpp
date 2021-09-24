#include "are_types_same.hpp"

template<typename T0, typename T1>
concept type_same_as = are_types_same<T0, T1>;

template<typename T0, typename T1>
concept type_not_same_as = !are_types_same<T0, T1>;