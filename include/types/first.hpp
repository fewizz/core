#pragma once

namespace types {

	class first {

		template<typename FirstType, typename... RemainingTypes>
		struct for_types_t { using type = FirstType; };

	public:

		template<typename... Types>
		using for_types = typename for_types_t<Types...>::type;

	};

} // types

template<typename... Types>
using first_type = types::first::for_types<Types...>;