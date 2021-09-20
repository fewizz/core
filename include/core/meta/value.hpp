#pragma once

namespace value {

template<auto Value>
struct of {
	static constexpr auto value = Value;
	using type = decltype(Value);
};

}