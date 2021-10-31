#pragma once

#include "of.hpp"

namespace values {
	struct first {
		template<auto FirstValue, auto... TailValues>
		static constexpr auto for_values_of = FirstValue;

		template<typename Type>
		struct __for;

		template<auto HeadValue, auto... Values>
		struct __for<values::of<HeadValue, Values...>> {
			static constexpr auto value = HeadValue;
		};

		template<typename Type>
		static constexpr auto for_values = __for<Type>::value;
	};
}