#pragma once

#include "at.hpp"
#include "values.hpp"

template<std::size_t... Indices>
struct indices;

template<std::size_t IndexFrom>
class from {
	
	template<std::size_t IndexTo>
	struct to_t {
		template<std::size_t Index>
		struct current;

		template<>
		struct current<IndexTo> {
			using type = indices<>;
		};

		template<std::size_t Index>
		struct current {
			using type = typename indices<Index>::template append_back<typename current<Index + 1>::type>;
		};
	};
public:

	template<std::size_t IndexTo>
	using to = typename to_t<IndexTo>::template current<IndexFrom>::type;
}; // from


template<std::size_t... Indices>
struct indices {
	static constexpr std::size_t size = sizeof...(Indices);

	// append back
	template<std::size_t... Indices0>
	using append_indices_back = indices<Indices..., Indices0...>;

	template<std::size_t Index>
	using append_index_back = indices<Indices..., Index>;
private:
	template<typename T>
	struct append_back_t;

	template<std::size_t... Indices0>
	struct append_back_t<indices<Indices0...>> {
		using type = indices<Indices..., Indices0...>;
	};
public:

	template<typename T>
	using append_back = typename append_back_t<T>::type;

	// append front
	template<std::size_t... Indices0>
	using append_indices_front = indices<Indices0..., Indices...>;

	template<std::size_t Index>
	using append_index_front = indices<Index, Indices...>;
private:
	template<typename T>
	struct append_front_t;

	template<std::size_t... Indices0>
	struct append_front_t<indices<Indices0...>> {
		using type = indices<Indices0..., Indices...>;
	};
public:

	template<typename T>
	using append_front = append_front_t<T>;

	// at
	template<std::size_t Index>
	static constexpr std::size_t at_index = value::at<Index>::template of_values<Indices...>;

	template<std::size_t... Indices0>
	using at_indices = indices<at_index<Indices0>...>;

private:
	template<typename T>
	struct at_t;

	template<std::size_t... Indices0>
	struct at_t<indices<Indices0...>> {
		using type = at_indices<Indices0...>;
	};
public:

	template<typename Of>
	using at = typename at_t<Of>::type;

	// erase at

	template<std::size_t Index>
	using erase_at_index =
		at<
			typename 
			from<0>
			::template to<Index>
			::template append_back<typename
				from<Index + 1>
				::template to<size>
			>
		>;
}; // of