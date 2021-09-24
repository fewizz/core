#pragma once

template<auto Value>
struct value_of {
	static constexpr auto value = Value;
	using type = decltype(Value);
};