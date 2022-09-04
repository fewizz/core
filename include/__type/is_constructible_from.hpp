#pragma once

template<typename Type, typename... Args>
concept constructible_from = __is_constructible(Type, Args...);