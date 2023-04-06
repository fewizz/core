#pragma once

#include "./variant.hpp"

template<typename Unexpected>
class unexpected {
	Unexpected value_;
public:
	unexpected(Unexpected&& value) : value_ { value } {}
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
		one_of{ expected_index, ::forward<Type>(expected) }
	{}

	template<typename PossibleUnexpectedType>
	constexpr expected(unexpected<PossibleUnexpectedType> unexpected) :
		one_of{ unexpected_index, ::forward<UnexpectedType>(unexpected.ref()) }
	{}

	constexpr bool is_unexpected () const {
		return one_of.index() == unexpected_index;
	}

	constexpr bool is_expected () const {
		return one_of.index() == expected_index;
	}

	constexpr const UnexpectedType&  get_unexpected() const &  {
		return one_of.template get_at<unexpected_index>();
	}
	constexpr       UnexpectedType&  get_unexpected()       &  {
		return one_of.template get_at<unexpected_index>();
	}
	constexpr const UnexpectedType&& get_unexpected() const && {
		return move(*this).one_of.template get_at<unexpected_index>();
	}
	constexpr       UnexpectedType&& get_unexpected()       && {
		return move(*this).one_of.template get_at<unexpected_index>();
	}

	constexpr const Type&  get_expected() const &  {
		return one_of.template get_at<expected_index>();
	}
	constexpr       Type&  get_expected()       &  {
		return one_of.template get_at<expected_index>();
	}
	constexpr const Type&& get_expected() const && {
		return move(*this).one_of.template get_at<expected_index>();
	}
	constexpr       Type&& get_expected()       && {
		return move(*this).one_of.template get_at<expected_index>();
	}

	template<typename Handler>
	const Type&  get_expected_or(Handler&& handler) const &  {
		if(is_unexpected()) { return handler(move(*this).get_unexpected()); }
		return get_expected();
	}
	template<typename Handler>
	      Type&  get_expected_or(Handler&& handler)       &  {
		if(is_unexpected()) { return handler(get_unexpected()); }
		return get_expected();
	}
	template<typename Handler>
	const Type&& get_expected_or(Handler&& handler) const && {
		if(is_unexpected()) { return handler(move(*this).get_unexpected()); }
		return move(*this).get_expected();
	}
	template<typename Handler>
	      Type&& get_expected_or(Handler&& handler)       && {
		if(is_unexpected()) { return handler(move(*this).get_unexpected()); }
		return move(*this).get_expected();
	}

};