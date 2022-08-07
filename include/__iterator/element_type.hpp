#pragma once

#include "./basic.hpp"
#include "../expression_of_type.hpp"

template<basic_iterator Iterator>
using iterator_element_type = decltype(*expression_of_type<Iterator>);