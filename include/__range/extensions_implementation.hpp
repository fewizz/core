#pragma once

#include "./extensions.hpp"
#include "../__type/copy_const_ref.hpp"

#include "./contains.hpp"

template<typename Derived>
template<typename Element>
constexpr bool range_extensions<Derived>::
contains(this auto&& self, Element&& element) {
	return __range::contains(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Element>(element)
	);
}

#include "./copy.hpp"

template<typename Derived>
template<basic_range OtherRange>
constexpr void range_extensions<Derived>::copy_to(
	this auto&& self, OtherRange&& other_range
) {
	__range::copy {
		(copy_const_ref<decltype(self), Derived>&&) self
	}.to(
		forward<OtherRange>(other_range)
	);
}

template<typename Derived>
template<basic_output_stream<range_element_type<Derived>> OS>
constexpr void range_extensions<Derived>::copy_to(
	this auto&& self, OS&& output_stream
) {
	__range::copy {
		(copy_const_ref<decltype(self), Derived>&&) self
	}.to_output_stream(
		forward<OS>(output_stream)
	);
}

#include "../__ranges/have_equal_size_and_elements.hpp"

template<typename Derived>
template<basic_range OtherRange>
constexpr bool range_extensions<Derived>::has_equal_size_and_elements(
	this auto&& self, OtherRange&& other_range
) {
	return __ranges::have_equal_size_and_elements(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<OtherRange>(other_range)
	);
}

#include "./flat_view.hpp"

template<typename Derived>
constexpr auto range_extensions<Derived>::flat_view(this auto&& self) {
	return __range::flat_view {
		(copy_const_ref<decltype(self), Derived>&&) self
	};
}

#include "./dereference_view.hpp"

template<typename Derived>
constexpr auto range_extensions<Derived>::dereference_view(
	this auto&& self
) {
	return __range::dereference_view {
		(copy_const_ref<decltype(self), Derived>&&) self
	};
}

#include "./reverse_view.hpp"

template<typename Derived>
constexpr auto range_extensions<Derived>::reverse_view(
	this auto&& self
) {
	using T = copy_const_ref<decltype(self), Derived>&&;
	return __range::reverse_view<T> {
		(T) self
	};
}

#include "./indexed_view.hpp"

template<typename Derived>
constexpr auto range_extensions<Derived>::indexed_view(
	this auto&& self
) {
	return __range::indexed_view {
		(copy_const_ref<decltype(self), Derived>&&) self
	};
}

#include "./index_view.hpp"

template<typename Derived>
constexpr auto range_extensions<Derived>::index_view(
	this auto&& self
) {
	using index_type = range_element_index_type<Derived>;

	return __range::index_view {
		.end_ = index_type(
			::range_size((copy_const_ref<decltype(self), Derived>&&) self)
		)
	};
}

#include "./filter_view.hpp"

template<typename Derived>
template<typename Predicate>
constexpr auto range_extensions<Derived>::
filter_view(this auto&& self, Predicate&& p) {
	return __range::filter_view {
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(p)
	};
}

#include "../__ranges/transform_view.hpp"

template<typename Derived>
template<typename F>
constexpr auto range_extensions<Derived>::
transform_view(this auto&& self, F&& f) {
	return __ranges::transform_view_t {
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<F>(f)
	};
}

#include "./view_copied_elements_on_stack.hpp"

template<typename Derived>
template<typename Handler>
decltype(auto)
range_extensions<Derived>::view_copied_elements_on_stack(
	this auto&& self, Handler&& handler
) {
	return __range::view_copied_elements_on_stack(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Handler>(handler)
	);
}

#include "./try_find_first_satisfying.hpp"

template<typename Derived>
template<typename Predicate>
auto range_extensions<Derived>::try_find_first_satisfying(
	this auto&& self, Predicate&& predicate
) {
	return __range::try_find_first_satisfying(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(predicate)
	);
}

template<typename Derived>
template<typename Predicate>
requires reversable_range<Derived>
auto range_extensions<Derived>::try_find_last_satisfying(
	Predicate&& predicate
) const {
	return __range::try_find_first_satisfying(
		reverse_view(),
		forward<Predicate>(predicate)
	);
}

#include "./try_find_index_of_first_satisfying.hpp"

template<typename Derived>
template<typename Predicate>
auto range_extensions<Derived>::try_find_index_of_first_satisfying(
	this auto&& self, Predicate&& predicate
) {
	return __range::try_find_index_of_first_satisfying(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(predicate)
	);
}

#include "./try_find_index_of_last_satisfying.hpp"

template<typename Derived>
template<typename Predicate>
auto range_extensions<Derived>::try_find_index_of_last_satisfying(
	this auto&& self, Predicate&& predicate
) {
	return __range::try_find_index_of_last_satisfying(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<Predicate>(predicate)
	);
}

#include "../iterator_and_sentinel.hpp"

template<typename Derived>
auto range_extensions<Derived>::shrink_view(
	this auto&& self,
	auto size
) {
	auto i = ::range_iterator((copy_const_ref<decltype(self), Derived>&) self);
	return iterator_and_sentinel {i, i + size}.as_range();
}

#include "./split_view.hpp"

template<typename Derived>
template<typename... Splitters>
constexpr auto range_extensions<Derived>::split_view(
	this auto&& self,
	Splitters&&... splitters
) {
	return __range::split_view {
		(copy_const_ref<decltype(self), Derived>&&) self,
		::forward<Splitters>(splitters)...
	};
}

#include "./starts_with.hpp"

template<typename Derived>
template<typename... With>
bool range_extensions<Derived>::starts_with(
	this auto&& self, With&&... with
) {
	return __range::starts_with(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<With>(with)...
	);
}

#include "./ends_with.hpp"

template<typename Derived>
template<typename... With>
constexpr bool
range_extensions<Derived>::ends_with(
	this auto&& self, With&&... with
) {
	return __range::ends_with(
		(copy_const_ref<decltype(self), Derived>&&) self,
		forward<With>(with)...
	);
}

template<typename Derived>
template<typename Self>
constexpr nuint
range_extensions<Derived>::get_or_compute_size(this Self&& self) {
	auto i = ::range_iterator((copy_const_ref<Self, Derived>&) self);
	auto s = ::range_sentinel((copy_const_ref<Self, Derived>&) self);
	return iterator_and_sentinel{ i, s }.get_or_compute_distance();
}