#pragma once

#include <iterator>

namespace util {

// input_iterator
// TODO : wait for libc++ support
/*template<class It>
struct is_input_iterator : std::false_type  {};
template<std::input_iterator It>
struct is_input_iterator<It> : std::true_type {};

template<class It>
static constexpr bool is_input_iterator_v = is_input_iterator<It>::value;

// forward_iterator
template<class It>
struct is_forward_iterator : std::false_type  {};

template<std::forward_iterator It>
struct is_forward_iterator<It> : std::true_type {};

template<class It>
static constexpr bool is_forward_iterator_v = is_forward_iterator<It>::value;*/

}