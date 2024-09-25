#pragma once

#include "./variant.hpp"
#include "./forward.hpp"

template<typename Unexpected>
class unexpected {
	Unexpected value_;
public:
	unexpected(Unexpected&& value) : value_ { forward<Unexpected>(value) } {}
	Unexpected& ref() { return value_; }
};

template<typename Type>
unexpected(Type&&) -> unexpected<Type>;

template<typename Type, typename UnexpectedType>
class expected {
	static constexpr auto expected_index = variant_index<0>;
	static constexpr auto unexpected_index = variant_index<1>;
	variant<Type, UnexpectedType> one_of;

public:

	template<typename... Args>
	requires(
		!(sizeof...(Args) == 1 && types_are_same<expected, first_type<Args...>>)
	)
	constexpr expected(Args&&... args) :
		one_of{ forward<Args>(args)... }
	{}

	template<typename PossibleExpectedType>
	requires same_as<Type, UnexpectedType>
	constexpr expected(PossibleExpectedType&& expected) :
		one_of{ expected_index, ::forward<PossibleExpectedType>(expected) }
	{}

	template<typename PossibleUnexpectedType>
	constexpr expected(unexpected<PossibleUnexpectedType>&& unexpected) :
		one_of{ unexpected_index, ::forward<UnexpectedType>(unexpected.ref()) }
	{}

	constexpr bool is_unexpected () const {
		return one_of.index() == unexpected_index;
	}

	constexpr bool is_expected () const {
		return one_of.index() == expected_index;
	}

	constexpr explicit operator bool () const {
		return is_expected();
	}

	template<typename Self>
	constexpr decltype(auto) get_unexpected(this Self&& self) {
		return (forward<Self>(self).one_of).template get_at<unexpected_index>();
	}

	template<typename Self>
	constexpr decltype(auto) get_expected(this Self&& self) {
		return (forward<Self>(self).one_of).template get_at<expected_index>();
	}

	constexpr decltype(auto) move_expected(this auto&& self) {
		return (::move(self).one_of).template get_at<expected_index>();
	}

	constexpr decltype(auto) move_unexpected(this auto&& self) {
		return (::move(self).one_of).template get_at<unexpected_index>();
	}

	template<typename Self>
	constexpr decltype(auto) get(this Self&& self)
	requires same_as<Type, UnexpectedType> {
		return self.is_expected() ?
			forward<Self>(self).get_expected() :
			forward<Self>(self).get_unexpected();
	}

	template<typename Self>
	constexpr decltype(auto) move(this Self&& self)
	requires same_as<Type, UnexpectedType> {
		return self.is_expected() ?
			::move(self).get_expected() :
			::move(self).get_unexpected();
	}

	template<typename Handler, typename Self>
	const Type&  get_expected_or(this Self&& self, Handler&& handler) {
		if (self.is_unexpected()) {
			return handler(forward<Self>(self).get_unexpected());
		}
		return forward<Self>(self).get_expected();
	}

};