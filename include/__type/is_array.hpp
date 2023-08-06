#pragma once

template<typename Type>
inline constexpr bool type_is_array = __is_array(Type);