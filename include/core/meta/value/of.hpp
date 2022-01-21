#pragma once

namespace value {

	template<auto Value>
	struct of {

		static constexpr auto value = Value;

		using value_type = decltype(Value);

	};

}