#pragma once

#include "./extensions.hpp"
#include "../__type/copy_const_ref.hpp"

#include "./contains.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Element>
constexpr bool range_extensions<Derived, Options>::
contains(this auto&& self, Element&& element) {
	return __range::contains(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Element>(element)
	);
}

#include "./copy.hpp"

template<typename Derived, range_extensions_options Options>
template<basic_range OtherRange>
constexpr void range_extensions<Derived, Options>::copy_to(
	this auto&& self, OtherRange&& other_range
) {
	__range::copy {
		(copy_const_ref<decltype(self), Derived>&&) self
	}.to(
		forward<OtherRange>(other_range)
	);
}

template<typename Derived, range_extensions_options Options>
template<typename OS>
constexpr void range_extensions<Derived, Options>::copy_to(
	this auto&& self, OS&& output_stream
) {
	__range::copy {
		(copy_const_ref<decltype(self), Derived>&&) self
	}.to_output_stream(
		forward<OS>(output_stream)
	);
}

#include "../__ranges/have_equal_size_and_elements.hpp"

template<typename Derived, range_extensions_options Options>
template<basic_range OtherRange>
constexpr bool range_extensions<Derived, Options>::has_equal_size_and_elements(
	this auto&& self, OtherRange&& other_range
) {
	return __ranges::have_equal_size_and_elements(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<OtherRange>(other_range)
	);
}

#include "./flat_view.hpp"

template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::flat_view(this auto&& self) {
	return __range::flat_view {
		(copy_const_ref<decltype(self), Derived>&&) self
	};
}

#include "./dereference_view.hpp"

template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::dereference_view(
	this auto&& self
) {
	return __range::dereference_view {
		(copy_const_ref<decltype(self), Derived>&&) self
	};
}

#include "./reverse_view.hpp"

template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::reverse_view(
	this auto&& self
) {
	return __range::reverse_view {
		(copy_const_ref<decltype(self), Derived>&&) self
	};
}

#include "./filter_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
constexpr auto range_extensions<Derived, Options>::
filter_view(this auto&& self, Predicate&& p) {
	return __range::filter_view {
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(p)
	};
}

#include "../__ranges/transform_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename F>
constexpr auto range_extensions<Derived, Options>::
transform_view(this auto&& self, F&& f) {
	return __ranges::transform_view_t {
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<F>(f)
	};
}

#include "./view_copied_elements_on_stack.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Handler>
decltype(auto)
range_extensions<Derived, Options>::view_copied_elements_on_stack(
	this auto&& self, Handler&& handler
) {
	return __range::view_copied_elements_on_stack(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Handler>(handler)
	);
}

#include "./try_find_first_satisfying.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
auto range_extensions<Derived, Options>::try_find_first_satisfying(
	this auto&& self, Predicate&& predicate
) {
	return __range::try_find_first_satisfying(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(predicate)
	);
}

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
requires reversable_range<Derived>
auto range_extensions<Derived, Options>::try_find_last_satisfying(
	Predicate&& predicate
) const {
	return __range::try_find_first_satisfying(
		reverse_view(),
		forward<Predicate>(predicate)
	);
}

#include "./try_find_index_of_first_satisfying.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
auto range_extensions<Derived, Options>::try_find_index_of_first_satisfying(
	this auto&& self, Predicate&& predicate
) {
	return __range::try_find_index_of_first_satisfying(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(predicate)
	);
}

#include "./try_find_index_of_last_satisfying.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
auto range_extensions<Derived, Options>::try_find_index_of_last_satisfying(
	this auto&& self, Predicate&& predicate
) {
	return __range::try_find_index_of_last_satisfying(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(predicate)
	);
}

#include "../iterator_and_sentinel.hpp"

template<typename Derived, range_extensions_options Options>
auto range_extensions<Derived, Options>::shrink_view(
	this auto&& self,
	auto size
) {
	auto i = ::range_iterator((copy_const_ref<decltype(self), Derived>&) self);
	return iterator_and_sentinel {i, i + size}.as_range();
}

#include "./starts_with.hpp"

template<typename Derived, range_extensions_options Options>
template<typename... With>
bool range_extensions<Derived, Options>::starts_with(
	this auto&& self, With&&... with
) {
	return __range::starts_with(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<With>(with)...
	);
}

#include "./ends_with.hpp"

template<typename Derived, range_extensions_options Options>
template<typename... With>
constexpr bool
range_extensions<Derived, Options>::ends_with(
	this auto&& self, With&&... with
) {
	return __range::ends_with(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<With>(with)...
	);
}

template<typename Derived, range_extensions_options Options>
template<typename Self>
constexpr nuint
range_extensions<Derived, Options>::get_or_compute_size(this Self&& self) {
	auto i = ::range_iterator((copy_const_ref<Self, Derived>&) self);
	auto s = ::range_sentinel((copy_const_ref<Self, Derived>&) self);
	return iterator_and_sentinel{ i, s }.get_or_compute_distance();
}