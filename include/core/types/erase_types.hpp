#pragma once

#include "erase_type.hpp"

namespace types {

	template<typename... TypesToErase>
	class erase_types {

		template<typename ResultingTypes>
		struct resulting_types {

			template<typename... TypesToEraseRemaining>
			struct types_to_erase_remaining;

			template<typename TypeToErase>
			struct types_to_erase_remaining<TypeToErase> {
				using result = typename ResultingTypes::template pass_for_type<types::erase_type<TypeToErase>>;
			};

			template<typename TypeToErase, typename... TypesToEraseTail>
			struct types_to_erase_remaining<TypeToErase, TypesToEraseTail...> {

				using erased = typename ResultingTypes::template pass_for_type<types::erase_type<TypeToErase>>;

				using result = typename resulting_types<erased>::template types_to_erase_remaining<TypesToEraseTail...>::result;
			};
		};
	public:
		template<typename... Types>
		using for_types_of = typename resulting_types<types::of<Types...>>::template types_to_erase_remaining<TypesToErase...>::result;
	};

}