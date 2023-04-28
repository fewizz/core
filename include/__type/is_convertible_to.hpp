#pragma once

template<typename From, typename To>
inline constexpr bool is_convertible_to = __is_convertible_to(From, To);

template<typename From, typename To>
concept convertible_to = __is_convertible_to(From, To);