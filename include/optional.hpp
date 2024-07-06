#pragma once

#include "./variant.hpp"
#include "./__type/copy_const_ref.hpp"

namespace __optional {
	constexpr inline struct no_t{} no{};
}

template<typename Derived, typename... Types>
class optional_extensions {
	static constexpr bool single = sizeof...(Types) == 1;
	using first = first_type<Types...>;

	template<typename Self>
	decltype(auto) derived(this Self&& self) {
		return (copy_const_ref<Self, Derived>) self;
	}

public:

	bool has_value()    const { return  derived().has_value(); }
	bool has_no_value() const { return !derived().has_value(); }

	explicit operator bool () const { return has_value(); }

	template<typename Handler, typename Self>
	copy_const_ref<Self, Derived> if_has_no_value(
		this Self&& self, Handler&& handler
	) {
		if (self.has_no_value()) {
			handler();
		}
		return forward<Self>(self).derived();
	}

	template<typename Self>
	copy_const_ref<Self, first> get(this Self&& self) requires single {
		return forward<Self>(self).derived().get();
	}


	const remove_reference<first>*
	operator -> () const & requires single { return &get(); }
	      remove_reference<first>*
	operator -> ()       & requires single { return &get(); }

	template<typename Handler, typename Self>
	requires single
	decltype(auto) if_has_value(this Self&& self, Handler&& handler) {
		if (self.has_value()) {
			if constexpr(
				invokable_with<Handler, decltype(forward<Self>(self).get())>
			) {
				return handler(forward<Self>(self).get());
			}
			else {
				return handler();
			}
		}
	}

	template<typename Handler>
	requires single
	Derived& set_if_has_no_value(Handler&& handler) & {
		if (has_no_value()) {
			derived() = handler();
		}
		return derived();
	}

	void reset() {
		derived() = __optional::no;
	}
};

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
	requires assignable<base_type, Arg&&>
	constexpr optional& operator = (Arg&& arg) {
		base_type::template operator = <Arg> (::forward<Arg>(arg));
		return *this;
	}

	constexpr optional() : base_type{ __optional::no } {}

	using base_type::get_same_as;
	using base_type::is_same_as;

	template<typename Handler, typename Self>
	constexpr decltype(auto) view(this Self&& self, Handler&& handler) {
		((copy_const_ref<Self, base_type>) self)
		.view([&]<typename Type>(Type& v) {
			if constexpr (same_as<remove_const<Type>, __optional::no_t>) {
				__builtin_unreachable();
			}
			else { return handler(v); }
		});
	}

	constexpr bool has_value() const {
		return !base_type::template is_same_as<__optional::no_t>();
	}

	template<typename Self>
	constexpr decltype(auto) get(this Self&& self) requires single {
		return ((copy_const_ref<Self, base_type>) self)
			.template get_same_as<first>();
	}

	constexpr auto&& move(this auto&& self) requires single {
		return ::move(self).template get_same_as<first>();
	}

};

template<typename Type>
class optional<Type&> : public optional_extensions<optional<Type&>, Type> {
	Type* ptr_ = nullptr;
public:

	constexpr optional() {};
	constexpr ~optional() {
		ptr_ = nullptr;
	}

	constexpr optional(Type& element) : ptr_{ &element } {}

	constexpr optional& operator = (Type& element) {
		ptr_ = &element;
		return *this;
	}

	constexpr bool has_value() const { return ptr_ != nullptr; }

	constexpr auto& get(this auto&& self) { return *self.ptr_; }

	template<typename Self>
	constexpr decltype(auto) forward() {
		return ::forward<copy_const_ref<Self, Type>>(*ptr_);
	}

	constexpr auto* ptr(this auto& self) { return self.ptr_; }

};