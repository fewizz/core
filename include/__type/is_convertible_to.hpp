#pragma once

#include "./predicate_extensions.hpp"

template<typename From, typename To>
concept implicitly_convertible_to =
	__is_convertible_to(From, To);

template<typename From, typename To>
concept explicitly_convertible_to =
	! implicitly_convertible_to<From, To>
	&& requires (From&& f) { (To) f; };

template<typename From, typename To>
concept convertible_to =
	implicitly_convertible_to<From, To>
	|| explicitly_convertible_to<From, To>;


template<typename ToType>
struct is_convertible_to_t :
	type_predicate_extensions<is_convertible_to_t<ToType>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return convertible_to<Type, ToType>;
	}
	
};


template<typename ToType>
struct is_implicitly_convertible_to_t :
	type_predicate_extensions<is_implicitly_convertible_to_t<ToType>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return implicitly_convertible_to<Type, ToType>;
	}
	
};

template<typename ToType>
struct is_explicitly_convertible_to_t :
	type_predicate_extensions<is_explicitly_convertible_to_t<ToType>>
{

	template<typename Type>
	constexpr bool for_type() const {
		return explicitly_convertible_to<Type, ToType>;
	}
	
};

template<typename ToType>
constexpr is_convertible_to_t<ToType> is_convertible_to{};

template<typename ToType>
constexpr is_implicitly_convertible_to_t<ToType> is_implicitly_convertible_to{};

template<typename ToType>
constexpr is_explicitly_convertible_to_t<ToType> is_explicitly_convertible_to{};