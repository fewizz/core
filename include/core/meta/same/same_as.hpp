#include "are_same.hpp"

template<typename T0, typename T1>
concept same_as = are_same<T0, T1>;

template<typename T0, typename T1>
concept not_same_as = !are_same<T0, T1>;