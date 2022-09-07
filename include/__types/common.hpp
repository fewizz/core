#pragma once

#include "../__type/decay.hpp"
#include "../expression_of_type.hpp"

namespace __types {

	class common {

		template<typename... Types>
		struct result{  };

		template<typename Type>
		struct result<Type> : result<Type, Type> {};

		template<typename Type0, typename Type1>
		static constexpr bool case_0 =
				!__is_same_as(Type0, decay<Type0>) ||
				!__is_same_as(Type1, decay<Type1>);

		template<typename Type0, typename Type1>
		requires case_0<Type0, Type1>
		struct result<Type0, Type1> :
			result<decay<Type0>, decay<Type1>>
		{};

		template<typename Type0, typename Type1>
		static constexpr bool case_1 =
			! case_0<Type0, Type1> &&
			requires() {
				false ?
				expression_of_type<Type0> : expression_of_type<Type1>;

				requires requires() {
					typename decay<decltype(
						false ?
						expression_of_type<Type0> : expression_of_type<Type1>
					)>;
				};
			};

		template<typename Type0, typename Type1>
		requires case_1<Type0, Type1>
		struct result<Type0, Type1> {
			using type = decay<decltype(
				false ? expression_of_type<Type0> : expression_of_type<Type1>
			)>;
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