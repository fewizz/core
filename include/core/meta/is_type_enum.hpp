#pragma once

template<typename T>
constexpr inline bool is_type_enum = __is_enum(T);

template<typename T>
constexpr inline bool is_enum = is_type_enum<T>;