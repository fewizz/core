#pragma once

#include "./__types/predicate.hpp"  // IWYU pragma: export
#include "./__types/exclusively_satisfy_predicates.hpp"  // IWYU pragma: export
#include "./__types/are_same.hpp"  // IWYU pragma: export
#include "./__types/satisfy_predicates.hpp"  // IWYU pragma: export
#include "./__types/at_index.hpp"  // IWYU pragma: export
#include "./__types/at_indices.hpp"  // IWYU pragma: export
#include "./__types/common.hpp"  // IWYU pragma: export
#include "./__types/count_of_satisfying_predicate.hpp"  // IWYU pragma: export
#include "./__types/erase_at_index.hpp"  // IWYU pragma: export
#include "./__types/first.hpp"  // IWYU pragma: export
#include "./__types/index_of_satisfying_predicate.hpp"  // IWYU pragma: export
#include "./__types/indices_of_satisfying_predicate.hpp"  // IWYU pragma: export
#include "./__values/at_index.hpp"

template<typename... Types>
struct common_if_have {
	static constexpr bool have_common = false;
};

template<typename... Types>
requires requires { typename __types::common::for_types<Types...>; }
struct common_if_have<Types...> {
	static constexpr bool have_common = true;
	using common = __types::common::for_types<Types...>;
};

template<typename... Types>
struct types : common_if_have<Types...> {

	template<types_predicate auto... Predicates>
	static constexpr bool exclusively_satisfy_predicates =
		__types::exclusively_satisfy_predicates<Predicates...>::template
		for_types<Types...>;

	static constexpr bool are_same = types_are_same<Types...>;

	template<types_predicate auto... Predicates>
	static constexpr bool satisfy_predicates =
		__types::satisfy_predicates<Predicates...>::template
		for_types<Types...>;

	template<nuint Index>
	using at_index = type_at_index<Index, Types...>;

	template<nuint... Indices>
	using at_indices = types<__type_pack_element<Indices, Types...>...>;

	template<type_predicate auto Predicate>
	static constexpr nuint count_of
		= ::count_of_satisfying_predicate<Predicate>.template
		for_types<Types...>;

	template<nuint Index>
	using erase_at_index = typename
		__types::erase_at_index<Index>::template
		for_types<Types...>::template
		pass_for_type_directly<types>;

private:
	template<typename... Types0>
	struct erase_first_t {
		using type = void;
	};
	template<typename Type, typename... OtherTypes>
	struct erase_first_t<Type, OtherTypes...> {
		using type = types<OtherTypes...>;
	};
public:

	using erase_first = erase_first_t<Types...>::type;

	template<typename... Types0>
	using first = __types::first::for_types<Types..., Types0...>;

	template</*type_predicate*/ auto Predicate>
	static constexpr nuint index_of_satisfying_predicate =
		__types::index_of_satisfying_predicate<Predicate>::template
		for_types<Types...>;

	template</*type_predicate*/ auto Predicate>
	using indices_of_satisfying_predicate = typename
		__types::indices_of_satisfying_predicate<Predicate>::template
		for_types<Types...>;

	template<auto Predicate>
	struct _get {
		using _indices = types::indices_of_satisfying_predicate<Predicate>;
		static_assert(_indices::size == 1);
		using result = types::at_index<_indices::template at_index<0>>;
	};

	template<auto Predicate>
	using get = _get<Predicate>::result;

	template<auto Predicate, typename Fallback>
	struct _get_or {
		using result = Fallback;
	};

	template<auto Predicate, typename Fallback>
	requires ((Predicate == 1).template for_types<Types...>())
	struct _get_or<Predicate, Fallback> {
		using result = types::get<Predicate>;
	};

	template<auto Predicate, typename Fallback>
	using get_or = _get_or<Predicate, Fallback>::result;

	template<typename Predicate, typename Handler, typename DefaultHandler>
	decltype(auto) static inline
	constexpr view_first_satisfying_predicate_or_default(
		Predicate&& predicate,
		Handler&& handler,
		DefaultHandler&& default_handler
	) {
		using frst = first<>;
		if (predicate.template operator () <frst> ()) {
			return handler.template operator () <frst>();
		}
		if constexpr(sizeof...(Types) > 1) {
			return erase_first::view_first_satisfying_predicate_or_default(
				forward<Predicate>(predicate),
				forward<Handler>(handler),
				forward<DefaultHandler>(default_handler)
			);
		}

		return default_handler();
	}

};