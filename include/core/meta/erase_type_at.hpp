#pragma once

#include "../integer.hpp"
#include "types.hpp"

template <uint Index>
class erase_type_at {
	
	template<typename... LeftTypes>
	struct left_types {
		template<uint CurrentIndex>
		struct current_type_index {

			template<typename CurrentType, typename... RightTypes>
			struct right_types {
				using type = typename
					left_types<LeftTypes..., CurrentType>::
					template current_type_index<CurrentIndex + 1u>::
					template right_types<RightTypes...>
					::type;
			};
		};

		template<>
		struct current_type_index<Index> {
			template<typename CurrentType, typename... RightTypes>
			struct right_types {
				using type = types_of<LeftTypes..., RightTypes...>;
			};
		};
	};

public:

	template<typename... Types>
	using of = typename left_types<>::template current_type_index<0>::template right_types<Types...>::type;
};