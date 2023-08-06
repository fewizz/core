#pragma once

#include "./__variant/storage.hpp"

#include "./__type/is_same_as.hpp"
#include "./__type/is_assignable.hpp"
#include "./__type/is_constructible_from.hpp"
#include "./__type/is_list_constructible_from.hpp"
#include "./__type/is_trivial.hpp"
#include "./__type/is_base.hpp"
#include "./__values/first.hpp"

#include "./types.hpp"

#include "./forward.hpp"
#include "./placement_new.hpp"
#include "./move.hpp"
#include "./type.hpp"

template<nuint Index>
struct variant_index_t {
	constexpr operator nuint () const { return Index; }
};

template<nuint Index>
static constexpr variant_index_t<Index> variant_index{};

template<typename... Types>
class variant {
	using storage_type = __variant::storage<Types...>;
	using index_type = uint_for_states_count<sizeof...(Types)>;
	storage_type storage_{};
	index_type current_;

	template<auto TypePredicate>
	static constexpr nuint count_of_satisfying_predicate =
		types<Types...>::template count_of_satisfying_predicate<
			TypePredicate
		>;

	template<auto TypePredicate>
	static constexpr nuint index_of_satisfying_predicate =
		types<Types...>::template index_of_satisfying_predicate<
			TypePredicate
		>;

	template<auto TypePredicate>
	using type_satisfying_predicate
		= type_at_index<index_of_satisfying_predicate<TypePredicate>, Types...>;

	template<typename Type>
	static constexpr bool has_one_same_as =
		count_of_satisfying_predicate<is_same_as<Type>> == 1;

	template<typename Type>
	static constexpr nuint index_of_same_as =
		index_of_satisfying_predicate<is_same_as<Type>>;

	template<typename... Args>
	static constexpr bool has_one_constructible_from =
		count_of_satisfying_predicate<is_constructible_from<Args...>> == 1;

	template<typename... Args>
	static constexpr nuint index_of_constructible_from =
		index_of_satisfying_predicate<is_constructible_from<Args...>>;

	template<typename... Args>
	static constexpr bool has_one_list_constructible_from =
		count_of_satisfying_predicate<is_list_constructible_from<Args...>> == 1;

	template<typename... Args>
	static constexpr nuint index_of_list_constructible_from =
		index_of_satisfying_predicate<is_list_constructible_from<Args...>>;

	template<typename Type>
	static constexpr bool has_one_assignable_and_constructible_from =
		count_of_satisfying_predicate<
			is_constructible_from<Type> && is_assignable<Type>
		> == 1;

	template<typename Type>
	static constexpr nuint index_of_assignable_and_constructible_from =
		index_of_satisfying_predicate<
			is_constructible_from<Type> && is_assignable<Type>
		>;

public:

	template<index_type Index>
	using type_at = type_at_index<Index, Types...>;

	// constructor
	template<typename Type>
	requires has_one_constructible_from<Type>
	constexpr variant(Type&& arg) :
		current_ { index_of_constructible_from<Type> }
	{
		storage_.template create<__variant::treat_type_as::value_type>(
			current_, ::forward<Type>(arg)
		);
	}

	template<typename Type>
	requires (
		!has_one_constructible_from<Type> &&
		has_one_list_constructible_from<Type>
	)
	constexpr variant(Type&& arg) :
		current_ { index_of_list_constructible_from<Type> }
	{
		storage_.template create<__variant::treat_type_as::value_type>(
			current_, ::forward<Type>(arg)
		);
	}

	template<typename Type>
	requires (
		!has_one_constructible_from<Type> &&
		!has_one_list_constructible_from<Type> &&
		has_one_same_as<Type>
	)
	constexpr variant(Type&& arg) :
		current_ { index_of_same_as<Type> }
	{
		storage_.template create<__variant::treat_type_as::value_type>(
			current_, ::forward<Type>(arg)
		);
	}

	template<nuint Index, typename... Args>
	requires constructible_from<type_at_index<Index, Types...>, Args...>
	constexpr variant(variant_index_t<Index>, Args&&... args) :
		current_ { Index }
	{
		storage_.template create<__variant::treat_type_as::value_type>(
			current_, ::forward<Args>(args)...
		);
	}

	// copy constructor
	constexpr variant(const variant& other) :
		current_ { other.current_ }
	{
		other.view([&](auto& element) {
			storage_.template create<
				__variant::treat_type_as::internal_type
			>(other.current_, element);
		});
	}

	// move constructor
	constexpr variant(variant&& other) :
		current_ { other.current_ }
	{
		other.view([&](auto& element) {
			storage_.template create<
				__variant::treat_type_as::value_type
			>(current_, ::move(element));
		});
	}

	// move assignment operator
	constexpr variant& operator = (variant&& other) {
		if(current_ == other.current_) {
			view_raw([&](auto& this_e) {
				other.view_raw([&](auto& other_e) {
					if constexpr(
						assignable<
							decltype(this_e), decltype(::move(other_e))
						>
					) {
						this_e = ::move(other_e);
					}
				});
			});
		}
		else {
			storage_.destroy(current_);
			current_ = other.current_;
			other.view_raw([&](auto& element) {
				storage_.template create<
					__variant::treat_type_as::internal_type
				>(current_, ::move(element));
			});
		}
		return *this;
	}

	// copy assignment operator
	constexpr variant& operator = (const variant& other) {
		if(current_ == other.current_) {
			view([&](auto& this_e) {
				other.view([&](auto& other_e) {
					if constexpr(
						assignable<
							decltype(this_e), decltype(other_e)
						>
					) {
						this_e = other_e;
					}
				});
			});
		}
		else {
			storage_.destroy(current_);
			current_ = other.current_;
			other.view_raw([&](auto& element) {
				storage_.init_raw(current_, element);
			});
		}
		return *this;
	}

	// assignment operator
	template<typename TypeToAssign>
	requires has_one_constructible_from<TypeToAssign>
	constexpr variant& operator = (TypeToAssign&& value) {
		nuint index = index_of_constructible_from<TypeToAssign>;
		if(index == current_) {
			view_raw([&]<typename ElementType>(ElementType& element) {
				if constexpr(
					assignable<
						ElementType,
						decltype(::forward<TypeToAssign>(value))
					>
				) {
					element = ::forward<TypeToAssign>(value);
				}
				else if constexpr(
					constructible_from<ElementType, TypeToAssign&&>
				){
					element.~ElementType();
					new (&element) ElementType(::forward<TypeToAssign>(value));
				}
			});
		}
		else {
			storage_.destroy(current_);
			current_ = index;
			storage_.template create<
				__variant::treat_type_as::value_type
			>(index, ::forward<TypeToAssign>(value));
		}
		return *this;
	}

	template<typename Type, typename...Args>
	requires constructible_from<Type, Args...>
	static constexpr variant make(Args&&... args) {
		return variant(
			index_of_constructible_from<Args...>,
			forward<Args>(args)...
		);
	}

	// assignment operator
	template<typename Type, typename Arg>
	requires (constructible_from<Type, Arg>)
	constexpr void assign(Arg&& arg) {
		nuint index = index_of_same_as<Type>;
		if(index == current_) {
			view_raw([&]<typename ElementType>(ElementType& element) {
				if constexpr(
					assignable<ElementType, Arg>
				) {
					element = ::forward<Arg>(arg);
				}
				else if constexpr(
					constructible_from<ElementType, Arg>
				){
					element.~ElementType();
					new (&element) ElementType(::forward<Arg>(arg));
				}
			});
		}
		else {
			storage_.destroy(current_);
			current_ = index;
			storage_.template create<
				__variant::treat_type_as::value_type
			>(index, ::forward<Arg>(arg));
		}
	}

	constexpr ~variant() {
		storage_.destroy(current_);
	}

	template<typename Handler>
	decltype(auto) view(Handler&& handler) const {
		return storage_.template view<
			__variant::treat_type_as::value_type
		>(
			current_, ::move(handler)
		);
	}

	template<typename Handler>
	decltype(auto) view(Handler&& handler) {
		return storage_.template view<
			__variant::treat_type_as::value_type
		>(current_, ::move(handler));
	}

	template<typename Handler>
	decltype(auto) view_type(Handler&& handler) const {
		return view([&]<typename Type>(const Type&) -> decltype(auto) {
			return handler.template operator () <Type> ();
		});
	}

	template<typename Handler>
	decltype(auto) view_raw(Handler&& handler) const {
		return storage_.view_raw(current_, ::move(handler));
	}

	template<typename Handler>
	decltype(auto) view_raw(Handler&& handler) {
		return storage_.template view<
			__variant::treat_type_as::internal_type
		>(current_, ::move(handler));
	}

	constexpr nuint index() const {
		return current_;
	}

	template<typename Type>
	requires has_one_same_as<Type>
	constexpr bool is_same_as() const {
		return current_ == index_of_same_as<Type>;
	}

	template<nuint Index>
	constexpr const type_at<Index>&  get_at() const & {
		return storage_.template get_at<Index>();
	}
	template<nuint Index>
	constexpr       type_at<Index>&  get_at()       & {
		return storage_.template get_at<Index>();
	}
	template<nuint Index>
	constexpr const type_at<Index>&& get_at() const && {
		return move(storage_).template get_at<Index>();
	}
	template<nuint Index>
	constexpr       type_at<Index>&& get_at()       && {
		return move(storage_).template get_at<Index>();
	}

	template<auto TypePredicate>
	constexpr const type_satisfying_predicate<TypePredicate>&
	get_satisfying_predicate() const & {
		return storage_.template get_at<
			index_of_satisfying_predicate<TypePredicate>
		>();
	}
	template<auto TypePredicate>
	constexpr       type_satisfying_predicate<TypePredicate>&
	get_satisfying_predicate()       & {
		return storage_.template get_at<
			index_of_satisfying_predicate<TypePredicate>
		>();
	}
	template<auto TypePredicate>
	constexpr const type_satisfying_predicate<TypePredicate>&&
	get_satisfying_predicate() const && {
		return move(storage_).template get_at<
			index_of_satisfying_predicate<TypePredicate>
		>();
	}
	template<auto TypePredicate>
	constexpr       type_satisfying_predicate<TypePredicate>&&
	get_satisfying_predicate()       && {
		return move(storage_).template get_at<
			index_of_satisfying_predicate<TypePredicate>
		>();
	}

	template<typename Type>
	requires has_one_same_as<Type>
	constexpr const auto&  get_same_as() const & {
		return get_satisfying_predicate<::is_same_as<Type>>();
	}
	template<typename Type>
	requires has_one_same_as<Type>
	constexpr       auto&  get_same_as()       & {
		return get_satisfying_predicate<::is_same_as<Type>>();
	}
	template<typename Type>
	requires has_one_same_as<Type>
	constexpr const auto&& get_same_as() const && {
		return move(get_satisfying_predicate<::is_same_as<Type>>());
	}
	template<typename Type>
	requires has_one_same_as<Type>
	constexpr       auto&& get_same_as()       && {
		return move(get_satisfying_predicate<::is_same_as<Type>>());
	}

};