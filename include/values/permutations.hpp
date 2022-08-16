#pragma once

#include "./of.hpp"
#include "./at_index.hpp"
#include "./erase_at_index.hpp"
#include "./push_front.hpp"
#include "../types/combine_with.hpp"
#include "../integer.hpp"

namespace values {

	class permutations {

		template<auto Index>
		struct push_value_to_front {

			template<typename Type>
			using for_type = typename
				Type::template pass_for_type<
					values::push_front<Index>
				>;

		};

		template<auto... Values>
		struct vs {

			template<nuint CurrentIndex, typename Result>
			class current_index_and_result {

				using without_value_at_index = typename
					values::erase_at_index<CurrentIndex>::template
					for_values<Values...>;

				using inner_permutations = typename
					without_value_at_index::template
					pass_for_type<permutations>;

				static constexpr auto value_at_current_index =
					values::at_index<CurrentIndex>::template
					for_values<Values...>;

				using transformed = typename
					inner_permutations::template
					transform<push_value_to_front<value_at_current_index>>;

				using combine_with_transformed = typename
					transformed::template
					pass_for_type_directly<
						types::combine_with
					>;

				using combined_result = typename
					Result::template
					pass_for_type<combine_with_transformed>;
			public:

				using result = typename
					current_index_and_result<
						CurrentIndex + 1,
						combined_result
					>::result;

			};

			template<typename Result>
			struct current_index_and_result<sizeof...(Values), Result> {
				using result = Result;
			};

		};

		template<auto Value>
		struct vs<Value> {

			template<nuint CurrentIndex, typename Result>
			requires(CurrentIndex == 0)
			struct current_index_and_result {
				using result = types::of<values::of<Value>>;
			};

		};
	
	public:

		template<auto... Values>
		using for_values = typename
			vs<Values...>::template
			current_index_and_result<0, types::of<>>::
			result;

	};

} // values