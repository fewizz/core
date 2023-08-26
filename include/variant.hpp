#pragma once

#include "./__values/max.hpp"
#include "./types.hpp"
#include "./type.hpp"
#include "./move.hpp"
#include "./placement_new.hpp"

template<nuint Index>
struct variant_index_t {
	constexpr operator nuint () const { return Index; }
};

template<nuint Index>
static constexpr variant_index_t<Index> variant_index{};

template<typename... Types>
class variant {

	template<typename Type>
	using ptr_if_ref =
		if_satisfies<type_is_reference<Type>>
		::template then<remove_reference<Type>*>
		::template otherwise<Type>;

	using index_type = uint_for_states_count<sizeof...(Types)>;
	alignas(Types...) uint8 storage_[
		__values::max<sizeof(ptr_if_ref<Types>)...>
	];
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

	template<typename Type, typename... Args>
	requires constructible_from<Type, Args...>
	void construct(Args&&... args) {
		if constexpr(type_is_reference<Type>) {
			using ptr_type = remove_reference<Type>*;
			new(&storage_) ptr_type(&args...);
		}
		else {
			new(&storage_) Type(forward<Args>(args)...);
		}
	}

public:

	template<index_type Index>
	using type_at = type_at_index<Index, Types...>;

	// constructor
	template<typename Type>
	requires has_one_constructible_from<Type>
	constexpr variant(Type&& arg) :
		current_ { index_of_constructible_from<Type> }
	{
		using type = type_at<index_of_constructible_from<Type>>;
		construct<type>(forward<Type>(arg));
	}

	template<typename Type>
	requires (
		!has_one_constructible_from<Type> &&
		has_one_list_constructible_from<Type>
	)
	constexpr variant(Type&& arg) :
		current_ { index_of_list_constructible_from<Type> }
	{
		using type = type_at<index_of_list_constructible_from<Type>>;
		construct<type>(forward<Type>(arg));
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
		using type = type_at<index_of_same_as<Type>>;
		construct<type>(forward<Type>(arg));
	}

	template<nuint Index, typename... Args>
	requires constructible_from<type_at_index<Index, Types...>, Args...>
	constexpr variant(variant_index_t<Index>, Args&&... args) :
		current_ { Index }
	{
		using type = type_at<Index>;
		construct<type>(forward<Args>(args)...);
	}

	// copy constructor
	constexpr variant(const variant& other) :
		current_ { other.current_ }
	{
		other.view_raw([&]<typename Type>(Type& element) {
			new (&storage_) Type(element);
		});
	}

	// move constructor
	constexpr variant(variant&& other) :
		current_ { other.current_ }
	{
		other.view_raw([&]<typename Type>(Type& element) {
			new (&storage_) Type(move(element));
		});
	}

	// move assignment operator
	constexpr variant& operator = (variant&& other) {
		if(current_ == other.current_) {
			view([&](auto& this_e) {
				other.view([&](auto& other_e) {
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
			view_raw([]<typename Type>(Type& e){ e.~Type(); });
			current_ = other.current_;
			other.view_raw([&]<typename Type>(Type& element) {
				new (&storage_) Type(move(element));
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
			view_raw([]<typename Type>(Type& e){ e.~Type(); });
			current_ = other.current_;
			other.view_raw([&]<typename Type>(Type& element) {
				new (&storage_) Type(element);
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
			view_raw([]<typename Type>(Type& e){ e.~Type(); });
			current_ = index;
			construct<type_at<index_of_constructible_from<TypeToAssign>>>(
				forward<TypeToAssign>(value)
			);
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
			new (&storage_) Type(
				forward<Arg>(arg)
			);
		}
	}

	constexpr ~variant() {
		view_raw([]<typename Type>(Type& e) {
			e.~Type();
		});
	}

	template<typename Handler>
	decltype(auto) view(Handler&& handler) const {
		return view_raw([&]<typename RawType>(RawType& e) -> decltype(auto) {
			if constexpr(type_is_pointer<RawType>) {
				return handler(*e);
			}
			else {
				return handler(e);
			}
		});
	}

	template<typename Handler>
	decltype(auto) view(Handler&& handler) {
		return view_raw([&]<typename RawType>(RawType& e) -> decltype(auto) {
			if constexpr(type_is_pointer<RawType>) {
				return handler(*e);
			}
			else {
				return handler(e);
			}
		});
	}

	template<typename Handler, nuint Index>
	requires (Index < sizeof...(Types))
	decltype(auto) view_index_recursively(Handler&& handler) const {
		if (Index == current_) {
			return handler.template operator () <Index> ();
		}
		if constexpr(Index + 1 < sizeof...(Types)) {
			return view_type_recursively<Handler, Index + 1>(
				forward<Handler>(handler)
			);
		}
		__builtin_unreachable();
	}

	template<typename Handler>
	decltype(auto) view_index(Handler&& handler) const {
		switch (current_) {
			#define VIEW_INDEX_AT(index) \
			case index: if constexpr(index < sizeof...(Types)) \
				return handler.template operator () <index> ();
			VIEW_INDEX_AT(0)
			VIEW_INDEX_AT(1)
			VIEW_INDEX_AT(2)
			VIEW_INDEX_AT(3)
			VIEW_INDEX_AT(4)
			VIEW_INDEX_AT(5)
			VIEW_INDEX_AT(6)
			VIEW_INDEX_AT(7)
			VIEW_INDEX_AT(8)
			VIEW_INDEX_AT(9)
			VIEW_INDEX_AT(10)
			VIEW_INDEX_AT(11)
			VIEW_INDEX_AT(12)
			VIEW_INDEX_AT(13)
			VIEW_INDEX_AT(14)
			VIEW_INDEX_AT(15)
			VIEW_INDEX_AT(16)
			#undef VIEW_INDEX_AT
			default:
				if constexpr(17 < sizeof...(Types))
					return view_index_recursively<Handler, 17>(
						forward<Handler>(handler)
					);
		}
	}

	template<typename Handler>
	decltype(auto) view_type(Handler&& handler) const {
		return view_index([&]<nuint Index> -> decltype(auto) {
			using type = type_at<Index>;
			return handler.template operator () <type>();
		});
	}

	template<typename Handler>
	decltype(auto) view_raw_type(Handler&& handler) const {
		return view_index([&]<nuint Index> -> decltype(auto) {
			using type = type_at<Index>;
			using raw_type = ptr_if_ref<type>;
			return handler.template operator () <raw_type>();
		});
	}

	template<typename Handler>
	decltype(auto) view_raw(Handler&& handler) const {
		return view_raw_type([&]<typename RawType> -> decltype(auto) {
			return handler(*(RawType*)storage_);
		});
	}

	template<typename Handler>
	decltype(auto) view_raw(Handler&& handler) {
		return view_type([&]<typename Type> -> decltype(auto) {
			using raw_type = ptr_if_ref<Type>;
			return handler(*(raw_type*)storage_);
		});
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
		using type = type_at<Index>;
		if constexpr(type_is_reference<type>) {
			return **(remove_reference<type>**) storage_;
		}
		else {
			return *(type*)storage_;
		}
	}
	template<nuint Index>
	constexpr       type_at<Index>&  get_at()       & {
		using type = type_at<Index>;
		if constexpr(type_is_reference<type>) {
			return **(remove_reference<type>**) storage_;
		}
		else {
			return *(type*)storage_;
		}
	}
	template<nuint Index>
	constexpr const type_at<Index>&& get_at() const && {
		using type = type_at<Index>;
		if constexpr(type_is_reference<type>) {
			return move(**(remove_reference<type>**) storage_);
		}
		else {
			return move(*(type*)storage_);
		}
	}
	template<nuint Index>
	constexpr       type_at<Index>&& get_at()       && {
		using type = type_at<Index>;
		if constexpr(type_is_reference<type>) {
			return move(**(remove_reference<type>**) storage_);
		}
		else {
			return move(*(type*)storage_);
		}
	}

	template<auto TypePredicate>
	constexpr const type_satisfying_predicate<TypePredicate>&
	get_satisfying_predicate() const & {
		return get_at<index_of_satisfying_predicate<TypePredicate>>();
	}
	template<auto TypePredicate>
	constexpr       type_satisfying_predicate<TypePredicate>&
	get_satisfying_predicate()       & {
		return get_at<index_of_satisfying_predicate<TypePredicate>>();
	}
	template<auto TypePredicate>
	constexpr const type_satisfying_predicate<TypePredicate>&&
	get_satisfying_predicate() const && {
		return move(*this).template get_at<
			index_of_satisfying_predicate<TypePredicate>
		>();
	}
	template<auto TypePredicate>
	constexpr       type_satisfying_predicate<TypePredicate>&&
	get_satisfying_predicate()       && {
		return move(*this).template get_at<
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