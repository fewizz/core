#pragma once

#include "../integer.hpp"

namespace __values {

	template<auto... Values>
	struct of;

}

namespace indices {

	template<nuint... Values>
	using of = __values::of<Values...>;

	template<nuint FromIndex>
	class from {
	
		template<nuint ToIndex>
		struct to_index {
			static_assert(ToIndex >= FromIndex);

			static constexpr nuint size = ToIndex - FromIndex;

			template<nuint... ResultingIndices>
			struct resulting_indices;

			template<nuint... ResultingIndices>
			requires(size != sizeof...(ResultingIndices))
			struct resulting_indices<ResultingIndices...> {
				using type =
					typename resulting_indices<
						ResultingIndices...,
						FromIndex + sizeof...(ResultingIndices)
					>::type;
			};

			template<nuint... ResultingIndices>
			requires(size == sizeof...(ResultingIndices))
			struct resulting_indices<ResultingIndices...> {
				using type =
					indices::of<ResultingIndices...>;
			};

		};

		template<>
		struct to_index<FromIndex> {

			template<nuint...>
			struct resulting_indices { using type = indices::of<>; };

		};

	public:

		template<nuint ToIndex>
		using to = typename
			to_index<ToIndex>::template
			resulting_indices<>::type;

	}; // from

	template<>
	struct from<0> {

		template<typename Type, Type... Values>
		struct __integer_sequence {
			using values = __values::of<Values...>;
		};

		template<nuint ToIndex>
		using to =
			typename __make_integer_seq<
				__integer_sequence,
				nuint,
				ToIndex
			>::values;

	};

}

namespace __values {
	
	template<auto... Values>
	struct of {
		static constexpr nuint size = sizeof...(Values);
		static constexpr bool is_empty = size == 0;

		using indices = indices::from<0>::to<size>;

		template<typename To>
		static constexpr auto pass_for_value =
			To::template for_values<Values...>;

		template<typename To>
		using pass_for_type = typename To::template for_values<Values...>;

		template<typename Type>
		constexpr auto pass() const {
			return typename Type::template for_values<Values...>{};
		}

		template<template<auto...> typename Type>
		using pass_for_type_directly = Type<Values...>;

		template<typename Transformer>
		using transform = __values::of<
			Transformer::template for_value<Values>...
		>;

	};

} // values