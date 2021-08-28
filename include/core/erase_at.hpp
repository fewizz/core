#pragma once

#include <cstddef>

namespace values {
	template<auto... Values>
	struct of;
}

namespace types {
	template<typename... Types>
	struct of;
}

template <std::size_t Index>
class erase_at {
	
	template<typename... LeftTypes>
	struct left_types {
		template<std::size_t CurrentIndex>
		struct current_type_index {

			template<typename CurrentType, typename... RightTypes>
			struct right_types {
				using type = typename
					left_types<LeftTypes..., CurrentType>::
					template current_type_index<CurrentIndex + 1>::
					template right_types<RightTypes...>;
			};
		};

		template<>
		struct current_type_index<Index> {
			template<typename CurrentType, typename... RightTypes>
			struct right_types {
				using type = types::of<LeftTypes..., RightTypes...>;
			};
		};
	};

	template<auto... LeftValues>
	struct left_values {
		template<std::size_t CurrentIndex>
		struct current_value_index {

			template<auto CurrentValue, auto... RightValues>
			struct right_values {
				using type = typename
					left_values<LeftValues..., CurrentValue>::
					template current_values_index<CurrentIndex + 1>::
					template right_values<RightValues...>;
			};
		};

		template<>
		struct current_value_index<Index> {
			template<auto CurrentValue, auto... RightValues>
			struct right_values {
				using type = values::of<LeftValues..., RightValues...>;
			};
		};
	};

public:

	template<typename... Types>
	using of_types = typename left_types<>::template current_type_index<0>::template right_types<Types...>::type;
	
	template<auto... Values>
	using of_values = typename left_values<>::template current_value_index<0>::template right_values<Values...>::type;
}; // erase_at