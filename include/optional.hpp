#pragma once

#include "./variant.hpp"

template<typename Derived, typename... Types>
class optional_extensions {
	static constexpr bool single = sizeof...(Types) == 1;
	using first = first_type<Types...>;

	const Derived& derived() const { return (const Derived&)*this; }
	      Derived& derived()       { return (      Derived&)*this; }

public:

	bool has_value() const { return derived()->has_value(); }

	explicit operator bool () const { return has_value(); }

	template<typename Handler>
	const Derived& if_no_value(Handler&& handler) const {
		if(!has_value()) { handler(); }
		return derived();
	}
	template<typename Handler>
	      Derived& if_no_value(Handler&& handler)       {
		if(!has_value()) { handler(); }
		return *this;
	}

	decltype(auto) value() const requires single { return derived().value(); }
	decltype(auto) value()       requires single { return derived().value(); }

	const remove_reference<first>*
	operator -> () const requires single { return &value(); }
	      remove_reference<first>*
	operator -> ()       requires single { return &value(); }

	template<typename Handler>
	requires single
	decltype(auto) if_has_value(Handler&& handler) const {
		if(derived().has_value()) return handler(value());
	}
	template<typename Handler>
	requires single
	decltype(auto) if_has_value(Handler&& handler)       {
		if(derived().has_value()) return handler(value());
	}

	template<typename Handler>
	requires single
	Derived& set_if_no_value(Handler&& handler) {
		if(!derived().has_value()) {
			*this = handler();
		}
		return derived();
	}
};

namespace __optional {
	constexpr inline struct no_t{} no{};
}

template<typename... Types>
requires (sizeof...(Types) > 0)
class optional :
	variant<Types..., __optional::no_t>,
	public optional_extensions<optional<Types...>, Types...>
{
	static constexpr bool single = sizeof...(Types) == 1;
	using first = first_type<Types...>;
	using base_type = variant<Types..., __optional::no_t>;
public:

	using base_type::base_type;

	//using base_type::operator = ;
	template<typename... Args>
	optional operator = (Args&&... args) {
		base_type::operator = (forward<Args>(args)...);
		return *this;
	}

	optional() : base_type{ __optional::no } {}

	using base_type::get;
	using base_type::is;

	template<typename Handler>
	decltype(auto) view(Handler&& handler) const {
		base_type::view([&]<typename Type>(Type& v) {
			if constexpr (same_as<Type, __optional::no_t>) {}
			else { return handler(v); }
		});
	}

	bool has_value() const {
		return !base_type::template is<__optional::no_t>();
	}

	decltype(auto) value() const requires single {
		return base_type::template get<first>();
	}
	decltype(auto) value()       requires single {
		return base_type::template get<first>();
	}
};

template<typename Type>
class optional<Type&> : public optional_extensions<optional<Type*>, Type>{
	Type* ptr_;
public:

	optional() : ptr_{ nullptr } {}

	optional(Type& element) : ptr_{ &element } {}

	bool has_value() const { return ptr_ != nullptr; }

	decltype(auto) value() const { return *ptr_; }
	decltype(auto) value()       { return *ptr_; }

	const remove_reference<Type>* ptr() const { return ptr_; }
	      remove_reference<Type>* ptr()       { return ptr_; }

};