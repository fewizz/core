#pragma once

#include "./copy_const.hpp"
#include "./copy_volatile.hpp"

template<typename From, typename To>
using copy_cv = copy_const<From, copy_volatile<From, To>>;