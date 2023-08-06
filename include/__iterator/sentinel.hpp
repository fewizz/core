#pragma once

#include "./basic.hpp"
#include "./random_access.hpp"

template<typename Type, typename Iterator>
concept sentinel_for =
	basic_iterator<Iterator> &&
	requires(Type o, Iterator it) {
		it == o; o == it;
		it != o; o != it;
	};