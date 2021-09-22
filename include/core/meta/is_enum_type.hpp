#pragma once

template<typename T>
constexpr inline bool is_enum_type = __is_enum(T);

template<typename T>
constexpr inline bool is_enum = is_enum_type<T>;