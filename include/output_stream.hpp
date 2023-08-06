#pragma once

#include "./expression_of_type.hpp"
#include "./endianness.hpp"
#include "./forward.hpp"
#include "./__iterator/basic.hpp"

template<typename ValueType, typename Type>
requires requires(ValueType value, Type&& os) {
	os.write(forward<ValueType>(value));
}
constexpr void write(ValueType value, Type&& os) {
	os.write(forward<ValueType>(value));
}

template<typename ValueType, basic_iterator Type>
constexpr void write(ValueType value, Type&& it) {
	*it++ = forward<ValueType>(value);
}

template<typename Type, typename ValueType>
concept basic_output_stream = requires(Type&& t, ValueType v) {
	::write<ValueType, Type>(forward<ValueType>(v), forward<Type>(t));
};