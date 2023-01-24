#pragma once

#include "./extensions.hpp"

#include "./contains.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Element>
constexpr bool range_extensions<Derived, Options>::
contains(Element&& element) const {
	return __range::contains(range_(), forward<Element>(element));
}

#include "./copy.hpp"

template<typename Derived, range_extensions_options Options>
template<basic_range OtherRange>
constexpr void range_extensions<Derived, Options>::copy_to(
	OtherRange&& other_range
) const & {
	__range::copy{ range_() }.to( forward<OtherRange>(other_range) );
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
constexpr auto range_extensions<Derived, Options>::flat_view() const &  {
	return __range::flat_view{ range_() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::flat_view()       &  {
	return __range::flat_view{ range_() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::flat_view() const && {
	return __range::flat_view{ forward<const Derived>(range_()) };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::flat_view()       && {
	return __range::flat_view{ forward<      Derived>(range_()) };
}

#include "./dereference_view.hpp"

template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::dereference_view() const &  {
	return __range::dereference_view{ range_() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::dereference_view()       &  {
	return __range::dereference_view{ range_() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::dereference_view() const && {
	return __range::dereference_view{ forward<const Derived>(range_()) };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::dereference_view()       && {
	return __range::dereference_view{ forward<      Derived>(range_()) };
}

#include "./reverse_view.hpp"

template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::reverse_view() const &  {
	return __range::reverse_view{ range_() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::reverse_view()       &  {
	return __range::reverse_view{ range_() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::reverse_view() const && {
	return __range::reverse_view{ forward<const Derived>(range_()) };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::reverse_view()       && {
	return __range::reverse_view{ forward<      Derived>(range_()) };
}

#include "./sized_view.hpp"

template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::sized_view() const &  {
	return __range::sized_view{ range_(), get_or_compute_size() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::sized_view()       &  {
	return __range::sized_view{ range_(), get_or_compute_size() };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::sized_view() const && {
	auto size = get_or_compute_size();
	return __range::sized_view{ forward<const Derived>(range_()), size };
}
template<typename Derived, range_extensions_options Options>
constexpr auto range_extensions<Derived, Options>::sized_view()       && {
	auto size = get_or_compute_size();
	return __range::sized_view{ forward<      Derived>(range_()), size };
}

#include "./filter_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Predicate>
constexpr auto range_extensions<Derived, Options>::
filter_view(Predicate&& p) const &  {
	return __range::filter_view{ range_(), forward<Predicate>(p) };
}
template<typename Derived, range_extensions_options Options>
template<typename Predicate>
constexpr auto range_extensions<Derived, Options>::
filter_view(Predicate&& p)       &  {
	return __range::filter_view{ range_(), forward<Predicate>(p) };
}
template<typename Derived, range_extensions_options Options>
template<typename Predicate>
constexpr auto range_extensions<Derived, Options>::
filter_view(Predicate&& p) const && {
	return __range::filter_view {
		forward<const Derived>(range_()), forward<Predicate>(p)
	};
}
template<typename Derived, range_extensions_options Options>
template<typename Predicate>
constexpr auto range_extensions<Derived, Options>::
filter_view(Predicate&& p)       && {
	return __range::filter_view {
		forward<      Derived>(range_()), forward<Predicate>(p)
	};
}

#include "../__ranges/transform_view.hpp"

template<typename Derived, range_extensions_options Options>
template<typename F>
constexpr auto range_extensions<Derived, Options>::
transform_view(F&& f) const &  {
	return __ranges::transform_view{ range_(), forward<F>(f) };
}
template<typename Derived, range_extensions_options Options>
template<typename F>
constexpr auto range_extensions<Derived, Options>::
transform_view(F&& f)       &  {
	return __ranges::transform_view{ range_(), forward<F>(f) };
}
template<typename Derived, range_extensions_options Options>
template<typename F>
constexpr auto range_extensions<Derived, Options>::
transform_view(F&& f) const && {
	return __ranges::transform_view {
		forward<const Derived>(range_()), forward<F>(f)
	};
}
template<typename Derived, range_extensions_options Options>
template<typename F>
constexpr auto range_extensions<Derived, Options>::
transform_view(F&& f)       && {
	return __ranges::transform_view {
		forward<      Derived>(range_()), forward<F>(f)
	};
}

#include "./view_copied_elements_on_stack.hpp"

template<typename Derived, range_extensions_options Options>
template<typename Handler>
decltype(auto)
range_extensions<Derived, Options>::view_copied_elements_on_stack(
	Handler&& handler
) const  &  {
	return __range::view_copied_elements_on_stack(
		range_(), forward<Handler>(handler)
	);
}
template<typename Derived, range_extensions_options Options>
template<typename Handler>
decltype(auto)
range_extensions<Derived, Options>::view_copied_elements_on_stack(
	Handler&& handler
)        &  {
	return __range::view_copied_elements_on_stack(
		range_(), forward<Handler>(handler)
	);
}
template<typename Derived, range_extensions_options Options>
template<typename Handler>
decltype(auto)
range_extensions<Derived, Options>::view_copied_elements_on_stack(
	Handler&& handler
) const  && {
	return __range::view_copied_elements_on_stack(
		forward<const Derived>(range_()), forward<Handler>(handler)
	);
}
template<typename Derived, range_extensions_options Options>
template<typename Handler>
decltype(auto)
range_extensions<Derived, Options>::view_copied_elements_on_stack(
	Handler&& handler
)        && {
	return __range::view_copied_elements_on_stack(
		forward<      Derived>(range_()), forward<Handler>(handler)
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