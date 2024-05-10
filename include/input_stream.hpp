#pragma once

#include "./__iterator/basic.hpp"
#include "./forward.hpp"
#include "./__type/is_same_as.hpp"

template<typename Type>
requires requires(Type&& is) { is.read(); }
constexpr decltype(auto) read(Type&& is) {
	return is.read();
}

template<basic_iterator Type>
constexpr decltype(auto) read(Type&& it) {
	return *it++;
}

template<typename ISType, typename ValueType>
concept basic_input_stream = requires(ISType&& is) {
	::read<ISType>(forward<ISType>(is));
};