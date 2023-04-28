#pragma once

#include "../__type/decay.hpp"
#include "../expression_of_type.hpp"

namespace __types {

	class common {

		template<typename... Types>
		struct result{  };

		template<typename Type>
		struct result<Type> {
			using type = Type;
		};

		template<typename Type0, typename Type1>
		requires requires() {
			false ?
				expression_of_type<Type0>() :
				expression_of_type<Type1>();
		}
		struct result<Type0, Type1> {
			using type = decltype(
				false ?
					expression_of_type<Type0>() :
					expression_of_type<Type1>()
			);
		};

		template<typename Type0, typename Type1, typename... Types>
		requires (sizeof...(Types) > 0) && requires {
			typename result<Type0, Type1>::type;
		}
		struct result<Type0, Type1, Types...> :
			result<typename result<Type0, Type1>::type, Types...>
		{};

	public:

		template<typename... Types>
		using for_types = typename result<Types...>::type;

	};

}