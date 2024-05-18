#pragma once

#include "./__values/max.hpp"
#include "./types.hpp"
#include "./type.hpp"
#include "./__type/copy_const_ref.hpp"
#include "./move.hpp"
#include "./placement_new.hpp"  // IWYU pragma: keep

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
	alignas(ptr_if_ref<Types>...) uint8 storage_[
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

	template<index_type Index>
	using type_at = type_at_index<Index, Types...>;

	template<index_type Index>
	using raw_type_at = ptr_if_ref<type_at<Index>>;

	template<nuint Index, typename... Args>
	void construct(Args&&... args) {
		using type = type_at<Index>;
		if constexpr(type_is_reference<type>) {
			using raw_type = ptr_if_ref<type>;
			new(storage_) raw_type(&args...);
		}
		else {
			new(storage_) type(forward<Args>(args)...);
		}
	}

	template<nuint Index>
	void destruct() {
		using raw_type = raw_type_at<Index>;
		((raw_type*)storage_)->~raw_type();
	}

	template<nuint Index>
	raw_type_at<Index>& raw_at() {
		using raw_type = raw_type_at<Index>;
		return *((raw_type*)storage_);
	}

public:

	// constructor
	template<typename Type>
	requires has_one_constructible_from<Type>
	constexpr variant(Type&& arg) :
		current_ { index_of_constructible_from<Type> }
	{
		construct<index_of_constructible_from<Type>>(forward<Type>(arg));
	}

	template<typename Type>
	requires (
		!has_one_constructible_from<Type> &&
		has_one_list_constructible_from<Type>
	)
	constexpr variant(Type&& arg) :
		current_ { index_of_list_constructible_from<Type> }
	{
		construct<index_of_list_constructible_from<Type>>(forward<Type>(arg));
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
		construct<index_of_same_as<Type>>(forward<Type>(arg));
	}

	template<nuint Index, typename... Args>
	requires constructible_from<type_at_index<Index, Types...>, Args...>
	constexpr variant(variant_index_t<Index>, Args&&... args) :
		current_ { Index }
	{
		construct<Index>(forward<Args>(args)...);
	}

	// copy constructor
	constexpr variant(const variant& other) :
		current_ { other.current_ }
	{
		other.view_index([&]<nuint Index> {
			construct<Index>(other.get_at<Index>());
		});
	}

	// move constructor
	constexpr variant(variant&& other) :
		current_ { other.current_ }
	{
		other.view_index([&]<nuint Index> {
			construct<Index>(move(other.get_at<Index>()));
		});
	}

	// move assignment operator
	constexpr variant& operator = (variant&& other) {
		variant& ths = *this;

		view_index([&]<nuint CurrentI> { other.view_index([&]<nuint OtherI> {
			if constexpr(CurrentI == OtherI) {
				ths.get_at<CurrentI>() = move(other.get_at<OtherI>());
			}
			else {
				ths.destruct<CurrentI>();
				ths.construct<OtherI>(move(other.raw_at<OtherI>()));
				ths.current_ = OtherI;
			}
		}); });
		return *this;
	}

	// copy assignment operator
	constexpr variant& operator = (const variant& other) {
		if (this == &other) {
			return *this;
		}

		variant& ths = *this;

		view_index([&]<nuint CurrentI> { other.view_index([&]<nuint OtherI> {
			if constexpr(CurrentI == OtherI) {
				ths.get_at<CurrentI>() = other.get_at<OtherI>();
			}
			else {
				ths.destruct<CurrentI>();
				ths.construct<OtherI>(other.raw_at<OtherI>());
				ths.current_ = OtherI;
			}
		}); });
		return ths;
	}

	// assignment operator
	template<typename Type>
	requires has_one_constructible_from<Type>
	constexpr variant& operator = (Type&& arg) {
		assign<index_of_constructible_from<Type>>(::forward<Type>(arg));
		return *this;
	}

	// assignment operator
	template<typename Type>
	requires (
		!has_one_constructible_from<Type> &&
		has_one_same_as<Type>
	)
	constexpr variant& operator = (Type&& arg) {
		assign<index_of_same_as<Type>>(forward<Type>(arg));
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
	template<nuint TypeIndex, typename Arg>
	//requires (constructible_from<type_at<TypeIndex>, Arg>)
	constexpr void assign(Arg&& arg) {
		view_index([&]<nuint CurrentI> {
			if constexpr(
				CurrentI == TypeIndex &&
				assignable<type_at<TypeIndex>, Arg>
			) {
				get_at<CurrentI>() = ::forward<Arg>(arg);
			}
			else {
				destruct<CurrentI>();
				construct<TypeIndex>(::forward<Arg>(arg));
				current_ = TypeIndex;
			}
		});
	}

	constexpr ~variant() {
		view_index([&]<nuint CurrentI> { destruct<CurrentI>(); });
	}

	template<typename Handler>
	constexpr decltype(auto) view(this auto&& self, Handler&& handler) {
		return self.view_raw([&]<typename RawType>(RawType& e)
		-> decltype(auto) {
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
			return view_index_recursively<Handler, Index + 1>(
				forward<Handler>(handler)
			);
		}
		__builtin_unreachable();
	}

	template<typename Handler>
	decltype(auto) view_index(Handler&& handler) const {
		switch (current_) {
			#define VIEW_INDEX_AT(index) \
			case index: if constexpr((index) < sizeof...(Types)) \
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
		__builtin_unreachable();
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
	decltype(auto) view_raw(this auto&& self, Handler&& handler) {
		return self.view_raw_type([&]<typename RawType> -> decltype(auto) {
			return handler(*(RawType*) self.storage_);
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

	template<nuint Index, typename Self>
	constexpr copy_const_ref<Self, type_at<Index>>&& get_at(this Self&& self) {
		using type = type_at<Index>;
		using resulting_type = copy_const_ref<Self, type>&&;

		if constexpr(type_is_reference<type>) {
			return (resulting_type) **(remove_reference<type>**)
			((copy_const_ref<Self, variant>&&) self).storage_;
		}
		else {
			return (resulting_type) *(type*)
			((copy_const_ref<Self, variant>&&) self).storage_;
		}
	}

	template<auto TypePredicate, typename Self>
	constexpr decltype(auto)
	get_satisfying_predicate(this Self&& self) {
		return ((copy_const_ref<Self, variant>&&) self)
		.template get_at<
			index_of_satisfying_predicate<TypePredicate>
		>();
	}

	template<typename Type, typename Self>
	requires has_one_same_as<Type>
	constexpr decltype(auto) get_same_as(this Self&& self) {
		return ((copy_const_ref<Self, variant>&&) self)
		.template get_satisfying_predicate<
			::is_same_as<Type>
		>();
	}

};