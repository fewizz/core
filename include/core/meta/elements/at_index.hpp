#pragma once

#include "of.hpp"
#include "../type/decay.hpp"
#include "../../forward.hpp"

namespace elements {

	template<nuint Index>
	struct at_index_t {

		template<typename... Types>
		constexpr decltype(auto) operator () (Types&&... elements) const {
			return elements::of {
				forward<Types>(elements)...
			}.template at<Index>();
		}
	};

	template<>
	struct at_index_t<0> {

		template<typename Type0, typename... Types>
		constexpr decltype(auto) operator () (Type0&& element, Types&&...) const {
			return forward<Type0>(element);
		}

	};

	template<>
	struct at_index_t<1> {

		template<typename Type0, typename Type1, typename... Types>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&& element, Types&&...
		) const {
			return forward<Type1>(element);
		}

	};

	template<>
	struct at_index_t<2> {

		template<
			typename Type0, typename Type1, typename Type2,
			typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&& element, Types&&...
		) const {
			return forward<Type2>(element);
		}

	};

	template<>
	struct at_index_t<3> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&& element,
			Types&&...
		) const {
			return forward<Type3>(element);
		}

	};

	template<>
	struct at_index_t<4> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& element, Types&&...
		) const {
			return forward<Type4>(element);
		}

	};

	template<>
	struct at_index_t<5> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&& element, Types&&...
		) const {
			return forward<Type5>(element);
		}

	};

	template<>
	struct at_index_t<6> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&& element, Types&&...
		) const {
			return forward<Type6>(element);
		}

	};

	template<>
	struct at_index_t<7> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&& element,
			Types&&...
		) const {
			return forward<Type7>(element);
		}

	};

	template<>
	struct at_index_t<8> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename Type8, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&&,
			Type8&& element, Types&&...
		) const {
			return forward<Type8>(element);
		}

	};

	template<>
	struct at_index_t<9> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename Type8, typename Type9, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&&,
			Type8&&, Type9&& element, Types&&...
		) const {
			return forward<Type9>(element);
		}

	};

	template<>
	struct at_index_t<10> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename Type8, typename Type9, typename Type10, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&&,
			Type8&&, Type9&&, Type10&& element, Types&&...
		) const {
			return forward<Type10>(element);
		}

	};

	template<>
	struct at_index_t<11> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename Type8, typename Type9, typename Type10, typename Type11,
			typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&&,
			Type8&&, Type9&&, Type10&&, Type11&& element,
			Types&&...
		) const {
			return forward<Type11>(element);
		}

	};

	template<>
	struct at_index_t<12> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename Type8, typename Type9, typename Type10, typename Type11,
			typename Type12, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&&,
			Type8&&, Type9&&, Type10&&, Type11&&,
			Type12&& element, Types&&...
		) const {
			return forward<Type12>(element);
		}

	};

	template<>
	struct at_index_t<13> {

		template<
			typename Type0, typename Type1, typename Type2, typename Type3,
			typename Type4, typename Type5, typename Type6, typename Type7,
			typename Type8, typename Type9, typename Type10, typename Type11,
			typename Type12, typename Type13, typename... Types
		>
		constexpr decltype(auto) operator () (
			Type0&&, Type1&&, Type2&&, Type3&&,
			Type4&& ,Type5&&, Type6&&, Type7&&,
			Type8&&, Type9&&, Type10&&, Type11&&,
			Type12&&, Type13&& element, Types&&...
		) const {
			return forward<Type13>(element);
		}

	};

	template<nuint Index>
	inline constexpr auto at_index = elements::at_index_t<Index>{};

} // elements