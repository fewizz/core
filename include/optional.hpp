#pragma once

#include "./variant.hpp"

template<typename Derived, typename... Types>
class optional_extensions {
	static constexpr bool single = sizeof...(Types) == 1;
	using first = first_type<Types...>;

	const Derived& derived() const { return (const Derived&) *this; }
	      Derived& derived()       { return (      Derived&) *this; }

public:

	bool has_value()    const { return  derived().has_value(); }
	bool has_no_value() const { return !derived().has_value(); }

	explicit operator bool () const { return has_value(); }

	template<typename Handler>
	const Derived&  if_has_no_value(Handler&& handler) const &  {
		if(has_no_value()) { handler(); }
		return derived();
	}
	template<typename Handler>
	      Derived&  if_has_no_value(Handler&& handler)       &  {
		if(has_no_value()) { handler(); }
		return derived();
	}
	template<typename Handler>
	const Derived&& if_has_no_value(Handler&& handler) const && {
		if(has_no_value()) { handler(); }
		return ::move(derived());
	}
	template<typename Handler>
	      Derived&& if_has_no_value(Handler&& handler)       && {
		if(has_no_value()) { handler(); }
		return ::move(derived());
	}

	const first&  get() const &  requires single { return derived().get(); }
	      first&  get()       &  requires single { return derived().get(); }
	const first&& get() const && requires single {
		return ::move(derived()).get();
	}
	      first&& get()       && requires single {
		return ::move(derived()).get();
	}

	const remove_reference<first>*
	operator -> () const & requires single { return &get(); }
	      remove_reference<first>*
	operator -> ()       & requires single { return &get(); }

	template<typename Handler>
	requires single
	decltype(auto) if_has_value(Handler&& handler) const {
		if(has_value()) { return handler(get()); }
	}
	template<typename Handler>
	requires single
	decltype(auto) if_has_value(Handler&& handler)       {
		if(has_value()) { return handler(get()); }
	}

	template<typename Handler>
	requires single
	Derived& set_if_has_no_value(Handler&& handler) & {
		if(has_no_value()) {
			derived() = handler();
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

	template<typename Arg>
	requires assignable<base_type, Arg>
	optional operator = (Arg&& args) {
		base_type::operator = (forward<Arg>(args));
		return *this;
	}

	optional() : base_type{ __optional::no } {}

	using base_type::get_same_as;
	using base_type::is_same_as;

	template<typename Handler>
	decltype(auto) view(Handler&& handler) const {
		base_type::view([&]<typename Type>(Type& v) {
			if constexpr (same_as<Type, __optional::no_t>) {}
			else { return handler(v); }
		});
	}

	bool has_value() const {
		return !base_type::template is_same_as<__optional::no_t>();
	}

	const first&  get() const &  requires single {
		return base_type::template get_same_as<first>();
	}
	      first&  get()       &  requires single {
		return base_type::template get_same_as<first>();
	}
	const first&& get() const && requires single {
		return ::move(*this).template get_same_as<first>();
	}
	      first&& get()       && requires single {
		return ::move(*this).template get_same_as<first>();
	}

	const remove_reference<first>&& move() const requires single {
		return ::move(*this).template get_same_as<first>();
	}
	      remove_reference<first>&& move()       requires single {
		return ::move(*this).template get_same_as<first>();
	}
};

template<typename Type>
class optional<Type&> : public optional_extensions<optional<Type&>, Type> {
	Type* ptr_ = nullptr;
public:

	optional() {};
	~optional() {
		ptr_ = nullptr;
	}

	optional(Type& element) : ptr_{ &element } {}

	optional& operator = (Type& element) { ptr_ = &element; return *this; }

	bool has_value() const { return ptr_ != nullptr; }

	const Type&  get() const { return *ptr_; }
	      Type&  get()       { return *ptr_; }

	const Type&& forward() const { return ::forward<const Type>(*ptr_); }
	      Type&& forward()       { return ::forward<      Type>(*ptr_); }

	const Type* ptr() const & { return ptr_; }
	      Type* ptr()       & { return ptr_; }

};