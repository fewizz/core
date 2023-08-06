#pragma once

#include "./__iterator/basic.hpp"
#include "./forward.hpp"

template<typename ValueType, typename Type>
requires requires(Type&& is) {
	is.template read<ValueType>();
}
constexpr ValueType read(Type&& is) {
	return is.template read<ValueType>();
}

template<typename ValueType, basic_iterator Type>
constexpr ValueType read(Type&& it) {
	return *it++;
}

template<typename Type, typename ValueType>
concept basic_input_stream = requires(Type&& t, ValueType&& v) {
	::read<ValueType, Type>(forward<Type>(t));
};