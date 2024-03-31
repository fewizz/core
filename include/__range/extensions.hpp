#pragma once

#include "./extensions_declaration.hpp"
#include "./sized.hpp"
#include "./reversable.hpp"
#include "./element_index_type.hpp"
#include "../loop_action.hpp"

template<typename Derived, range_extensions_options Options>
struct range_extensions {

private:
	constexpr const Derived&
	range_() const &  { return (const Derived&)  *this; }
	constexpr       Derived&
	range_()       &  { return (      Derived&)  *this; }
	constexpr const Derived&&
	range_() const && { return (const Derived&&) *this; }
	constexpr       Derived&&
	range_()       && { return (      Derived&&) *this; }

	constexpr basic_iterator auto _iterator() const {
		return range_iterator(range_());
	}
	constexpr basic_iterator auto _iterator()       {
		return range_iterator(range_());
	}
	constexpr auto _sentinel() const { return range_sentinel(range_()); }
	constexpr auto _sentinel()       { return range_sentinel(range_()); }

public:

	// for compatibility
	constexpr basic_iterator auto begin() const { return _iterator(); }
	constexpr basic_iterator auto begin()       { return _iterator(); }
	constexpr auto end() const { return _sentinel(); }
	constexpr auto end()       { return _sentinel(); }

	template<typename Handler>
	void for_each_indexed(Handler handler)       & {
		using index_type = range_element_index_type<Derived>;
		index_type i{};
		for(auto& e : range_()) {
			if constexpr(same_as<decltype(handler(e, i)), loop_action>) {
				if(handler(e, i) == loop_action::stop) {
					break;
				}
			}
			else {
				handler(e, i);
			}
			++i;
		}
	}
	template<typename Handler>
	void for_each_indexed(Handler handler) const & {
		using index_type = range_element_index_type<Derived>;
		index_type i{};
		for(auto& e : range_()) {
			if constexpr(same_as<decltype(handler(e, i)), loop_action>) {
				if(handler(e, i) == loop_action::stop) {
					break;
				}
			}
			else {
				handler(e, i);
			}
			++i;
		}
	}

	template<typename Handler>
	void for_each_index(Handler handler) const {
		using index_type = range_element_index_type<Derived>;
		auto size = range_size(range_());
		for(index_type i{}; i < size; ++i) {
			handler(i);
		}
	}

	template<typename Element>
	constexpr bool contains(Element&& element) const;

	template<basic_range OtherRange>
	constexpr void copy_to(OtherRange&& other_range) const &;

	template</*basic_output_stream*/typename OS>
	constexpr void copy_to(OS&& output_stream) const &;

	template<basic_range OtherRange>
	constexpr bool has_equal_size_and_elements(
		OtherRange&& other_range
	) const;

	constexpr auto flat_view(this auto&& self);

	constexpr auto dereference_view(this auto&& self);

	constexpr auto reverse_view(this auto&& self);

	constexpr auto sized_view(this auto&& self);

	template<typename Predicate>
	constexpr auto filter_view(this auto&& self, Predicate&&);

	template<typename F>
	constexpr auto transform_view(this auto&& self, F&&);

	template<typename Self>
	constexpr decltype(auto) operator [] (this Self&& self, nuint index) {
		return *(((range_extensions&&)self)._iterator() + index);
	}

	template<typename Handler>
	decltype(auto) view_copied_elements_on_stack(
		this auto&& self, Handler&& handler
	);

	template<typename Predicate>
	auto try_find_first_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_first_satisfying(Predicate&& predicate);

	template<typename Predicate>
	requires reversable_range<Derived>
	auto try_find_last_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_index_of_first_satisfying(Predicate&& predicate) const;

	template<typename Predicate>
	auto try_find_index_of_last_satisfying(Predicate&& predicate) const;

	auto shrink_view(auto size) const;

	template<typename... With>
	bool starts_with(With&&... with) const &;

	template<typename... With>
	constexpr bool ends_with(With&&... with) const &;

	constexpr nuint get_or_compute_size() const;

};