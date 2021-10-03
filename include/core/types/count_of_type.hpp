#pragma once

#include "../integer.hpp"
#include "indices_of_type.hpp"

namespace types {

	template<typename Type>
	struct count_of_type {

		template<typename... Types>
		static constexpr uint for_types_of =
			indices_of_type<Type>
			::template for_types_of<Types...>
			::size;
	
		template<uint N>
		struct less_or_equals {
			static constexpr bool is_types_predicate = true;

			template<typename... Types>
			static constexpr bool for_types_of = count_of_type::for_types_of<Types...> <= N;
		};
	
		template<uint N>
		struct equals {
			static constexpr bool is_types_predicate = true;
			
			template<typename... Types>
			static constexpr bool for_types_of = count_of_type::for_types_of<Types...> == N;
		};
	};

}