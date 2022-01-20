#pragma once

#include "../integer.hpp"
#include "../array.hpp"
#include "../elements/at_index.hpp"
#include "../types/count_of_type.hpp"
#include "../types/indices_of_type.hpp"
#include "../type/predicate.hpp"

namespace elements {

	template<typename TypeToView>
	struct view_t {

		template<nuint Count>
		struct acceptor {
			array<TypeToView, Count> m_storage;
			
			void operator () (auto&& f) const {
				f(m_storage);
			}

		};

		template<typename... Types>
		auto operator () (const Types&... elements) const {
			constexpr nuint count = types::count_of_type<TypeToView>::template for_types_of<Types...>;
			using indices_t = typename types::indices_of_type<TypeToView>::template for_types_of<Types...>;

			auto array_constructor = [&]<nuint... Indices>(indices::of<Indices...> indices = indices_t{}) {
				return array<TypeToView, count>{ elements::at_index<Indices>(elements)... };
			}();

			return acceptor{ array_constructor() };
		}

	};

	template<typename Type>
	inline constexpr auto view = view_t<Type>{};

}