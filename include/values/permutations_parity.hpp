#pragma once

#include "../integer.hpp"

namespace values {

	// TODO rename, redo
	struct permutations_parity {

		template<nuint Result, nuint... Indices>
		struct index {
			static constexpr nuint result = Result;
		};

		template<nuint Result, nuint Index, nuint... OtherIndices>
		requires(sizeof...(OtherIndices) > 0)
		struct index<Result, Index, OtherIndices...> {

			static constexpr nuint result =
				index<
					Result + ((Index > OtherIndices) + ...),
					OtherIndices...
				>::result;

		};

		template<nuint... Indices>
		static constexpr nuint for_indices = index<0, Indices...>::result;
	};

} // values