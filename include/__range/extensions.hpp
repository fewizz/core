#pragma once

#include "./extensions_declaration.hpp"
#include "./reversable.hpp"
#include "./element_index_type.hpp"
#include "./element_type.hpp"
#include "../__type/copy_const_ref.hpp"
#include "../__type/is_invokable_with.hpp"
#include "../output_stream.hpp"

template<typename Derived>
struct range_extensions {

	// for compatibility
	constexpr basic_iterator auto begin(this auto& self) {
		return ::range_iterator(
			(copy_const_ref<decltype(self), Derived>&&) self
		);
	}

	constexpr auto end(this auto& self) {
		return ::range_sentinel(
			(copy_const_ref<decltype(self), Derived>&&) self
		);
	}

	template<typename Element>
	constexpr bool contains(this auto&&, Element&& element);

	template<basic_range OtherRange>
	constexpr void copy_to(this auto&&, OtherRange&& other_range);

	template<basic_output_stream<range_element_type<Derived>> OS>
	constexpr void copy_to(this auto&&, OS&& output_stream);

	template<basic_range OtherRange>
	constexpr bool has_equal_size_and_elements(
		this auto&&, OtherRange&& other_range
	);

	constexpr auto flat_view(this auto&& self);

	constexpr auto dereference_view(this auto&& self);

	constexpr auto reverse_view(this auto&& self);

	constexpr auto indexed_view(this auto&& self);
	constexpr auto index_view(this auto&& self);

	template<typename Predicate>
	constexpr auto filter_view(this auto&& self, Predicate&&);

	template<typename F>
	constexpr auto transform_view(this auto&& self, F&&);

	constexpr decltype(auto) operator [] (this auto&& self, nuint index) {
		auto i = ::range_iterator(
			(copy_const_ref<decltype(self), Derived>&&) self
		);
		return *(i + index);
	}

	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(
		this auto&& self, Handler&& handler
	);

	template<typename Predicate>
	auto try_find_first_satisfying(this auto&&, Predicate&& predicate);

	template<typename Predicate>
	requires reversable_range<Derived>
	auto try_find_last_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_index_of_first_satisfying(this auto&&, Predicate&& predicate);

	template<typename Predicate>
	auto try_find_index_of_last_satisfying(this auto&&, Predicate&& predicate);

	auto shrink_view(this auto&& self, auto size);

	template<typename... Splitters>
	constexpr auto split_view(this auto&&, Splitters&&...);

	template<typename... With>
	bool starts_with(this auto&&, With&&... with);

	template<typename... With>
	constexpr bool ends_with(this auto&&, With&&... with);

	constexpr nuint get_or_compute_size(this auto&&);

};