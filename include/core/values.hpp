#pragma once

#include <cstddef>
#include "at.hpp"
#include "is.hpp"
#include "indices_of.hpp"
#include "erase_at.hpp"

namespace values {

template<auto... Values>
struct of;

}

namespace indices {

template<std::size_t IndexFrom>
class from {
	
	template<std::size_t IndexTo>
	struct to_t {
		static constexpr auto size = IndexTo - IndexFrom;

		template<std::size_t... Indices>
		struct result;

		template<std::size_t... Indices>
		requires(sizeof...(Indices) == size)
		struct result<Indices...> {
			using type = of<Indices...>;
		};

		template<std::size_t... Indices>
		requires(sizeof...(Indices) == 0 && size > 0)
		struct result<Indices...> {
			using type = typename result<IndexFrom>::type;
		};

		template<std::size_t... Indices>
		requires(sizeof...(Indices) > 0 && sizeof...(Indices) != size)
		struct result<Indices...> {
			using type = typename result<Indices..., of<Indices...>::back + 1>::type;
		};
	};
public:

	template<std::size_t IndexTo>
	using to = typename to_t<IndexTo>::template result<>::type;
}; // from

} // indices

namespace values {

template<>
struct of<> {
	static constexpr std::size_t size = 0;
	using indices = indices::from<0>::to<size>;

	// append back
	template<auto... Values>
	using append_back = of<Values...>;

	// append front
	template<auto... Values>
	using append_front = of<Values...>;
};

template<auto... Values>
struct of {
	static constexpr std::size_t size = sizeof...(Values);

	// append back
	template<auto... Values0>
	using append_back = of<Values..., Values0...>;

	// append front
	template<auto... Values0>
	using append_front = of<Values0..., Values...>;

	// at
	template<std::size_t Index>
	static constexpr auto at = ::at<Index>::template of_values<Values...>;

private:
	template<typename T>
	struct at_indices_t;

	template<std::size_t... Is>
	struct at_indices_t<of<Is...>> {
		using type = of<at<Is>...>;
	};
public:

	template<typename T>
	using at_indices = typename at_indices_t<T>::type;

	// front
	static constexpr auto front = at<0>;

	// back
	static constexpr auto back = at<size - 1>;

	// indices_of_same_as
	template<auto Value>
	using indices_of_same_as = typename
		indices::of_values_that_satisfy<
			is::value<Value>::template same_as_predicate
		>
		::template of_values<Values...>;

	// indices_of_not_same_as
	template<auto Value>
	using indices_of_not_same_as = typename
		indices::of_values_that_not_satisfy<
			is::value<Value>::template same_as_predicate
		>
		::template of_values<Values...>;

	// contains
	template<auto Value>
	using contains_predicate = std::bool_constant< (indices_of_same_as<Value>::size > 0) >;

	template<auto Value>
	static constexpr bool contains = contains_predicate<Value>::value;

	// erase_at
	template<std::size_t Index>
	using erase_at = typename erase_at<Index>::template of_values<Values...>;

/*private:
	template<typename T>
	struct erase_at_indices;

	template<std::size_t... Indices>
	struct erase_at_indices<indices::of<Indices...>> {
		using type = 
	}


	template< Index>
	using erase_at_indices = typename erase_at<Index>::template of_values<Values...>;*/
};

} // values