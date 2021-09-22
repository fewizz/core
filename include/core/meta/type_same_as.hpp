#include "are_types_same.hpp"

template<typename T0, typename T1>
concept type_same_as = are_same<T0, T1>;

template<typename T0, typename T1>
concept type_not_same_as = !are_same<T0, T1>;

template<typename T0, typename T1>
concept same_as = type_same_as<T0, T1>;

template<typename T0, typename T1>
concept not_same_as = type_not_same_as<T0, T1>;