#pragma once

#include <iterator>

namespace util {

template<class It>
struct is_input_iterator : std::integral_constant<bool, false>  {};

template<std::input_iterator It>
struct is_input_iterator<It> : std::integral_constant<bool, true> {};

template<class It>
static constexpr bool is_input_iterator_v = is_input_iterator<It>::value;

template<class It>
struct is_forward_iterator : std::integral_constant<bool, false>  {};

template<std::forward_iterator It>
struct is_forward_iterator<It> : std::integral_constant<bool, true> {};

template<class It>
static constexpr bool is_forward_iterator_v = is_forward_iterator<It>::value;
}