#pragma once

#include "./extensions.hpp"
#include "../__type/copy_const_ref.hpp"

#include "./contains.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Self, typename Element>
constexpr bool range_extensions<Derived, Options>::
contains(this Self&& self, Element&& element) {
	return __range::contains(
		((copy_const_ref<Self, range_extensions>&&) self).range_(),
		forward<Element>(element)
	);
}

#include "./copy.hpp"

template<typename Derived, range_extensions_options Options>
template<basic_range OtherRange>
constexpr void range_extensions<Derived, Options>::copy_to(
	OtherRange&& other_range
) const & {
	__range::copy{ range_() }.to( forward<OtherRange>(other_range) );
}

template<typename Derived, range_extensions_options Options>
template<typename OS>
constexpr void range_extensions<Derived, Options>::copy_to(
	OS&& output_stream
) const & {
	__range::copy{ range_() }.to_output_stream(forward<OS>(output_stream));
}

#include "../__ranges/have_equal_size_and_elements.hpp"

template<typename Derived, range_extensions_options Options>
template<basic_range OtherRange>
constexpr bool range_extensions<Derived, Options>::has_equal_size_and_elements(
	OtherRange&& other_range
) const {
	return __ranges::have_equal_size_and_elements(
		range_(), forward<OtherRange>(other_range)
	);
}

#include "./flat_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Self>
constexpr auto range_extensions<Derived, Options>::flat_view(this Self&& self) {
	return __range::flat_view {
		((copy_const_ref<Self, range_extensions>&&) self).range_()
	};
}

#include "./dereference_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Self>
constexpr auto range_extensions<Derived, Options>::dereference_view(
	this Self&& self
) {
	return __range::dereference_view {
		((copy_const_ref<Self, range_extensions>&&) self).range_()
	};
}

#include "./reverse_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Self>
constexpr auto range_extensions<Derived, Options>::reverse_view(
	this Self&& self
) {
	return __range::reverse_view {
		((copy_const_ref<Self, range_extensions>&&) self).range_()
	};
}

#include "./sized_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Self>
constexpr auto range_extensions<Derived, Options>::sized_view(
	this Self&& self
) {
	auto size = self.get_or_compute_size();
	return __range::sized_view {
		((copy_const_ref<Self, range_extensions>&&) self).range_(),
		size
	};
}

#include "./filter_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate, typename Self>
constexpr auto range_extensions<Derived, Options>::
filter_view(this Self&& self, Predicate&& p) {
	return __range::filter_view {
		forward<Self>(self).range_(),
		forward<Predicate>(p)
	};
}

#include "../__ranges/transform_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename F, typename Self>
constexpr auto range_extensions<Derived, Options>::
transform_view(this Self&& self, F&& f) {
	return __ranges::transform_view_t {
		((copy_const_ref<Self, range_extensions>&&) self).range_(),
		forward<F>(f)
	};
}

#include "./view_copied_elements_on_stack.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Handler, typename Self>
decltype(auto)
range_extensions<Derived, Options>::view_copied_elements_on_stack(
	this Self&& self, Handler&& handler
) {
	return __range::view_copied_elements_on_stack(
		((copy_const_ref<Self, range_extensions>&&) self).range_(),
		forward<Handler>(handler)
	);
}

#include "./try_find_first_satisfying.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
auto range_extensions<Derived, Options>::try_find_first_satisfying(
	Predicate&& predicate
) const {
	return __range::try_find_first_satisfying(
		range_(),
		forward<Predicate>(predicate)
	);
}

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
auto range_extensions<Derived, Options>::try_find_first_satisfying(
	Predicate&& predicate
) {
	return __range::try_find_first_satisfying(
		range_(),
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
	Predicate&& predicate
) const {
	return __range::try_find_index_of_first_satisfying(
		range_(),
		forward<Predicate>(predicate)
	);
}

#include "./try_find_index_of_last_satisfying.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
auto range_extensions<Derived, Options>::try_find_index_of_last_satisfying(
	Predicate&& predicate
) const {
	return __range::try_find_index_of_last_satisfying(
		range_(),
		forward<Predicate>(predicate)
	);
}

#include "../iterator_and_sentinel.hpp"

template<typename Derived, range_extensions_options Options>
auto range_extensions<Derived, Options>::shrink_view(auto size) const {
	return iterator_and_sentinel {
		_iterator(),
		_iterator() + size
	}.as_range();
}

#include "./starts_with.hpp"

template<typename Derived, range_extensions_options Options>
template<typename... With>
bool range_extensions<Derived, Options>::starts_with(With&&... with) const & {
	return __range::starts_with(range_(), forward<With>(with)...);
}

#include "./ends_with.hpp"

template<typename Derived, range_extensions_options Options>
template<typename... With>
constexpr bool
range_extensions<Derived, Options>::ends_with(With&&... with) const & {
	return __range::ends_with(range_(), forward<With>(with)...);
}

template<typename Derived, range_extensions_options Options>
constexpr nuint
range_extensions<Derived, Options>::get_or_compute_size() const {
	return iterator_and_sentinel {
		_iterator(), _sentinel()
	}.get_or_compute_distance();
}