#pragma once

#include "./elements/none.hpp"
#include "./variant.hpp"

template<typename Type>
class optional : public variant<Type, elements::none> {
	using base_type = variant<Type, elements::none>;
public:

	using base_type::base_type;
	using base_type::operator = ;

	optional() : base_type{ elements::none{} } {}

	bool has_value() const { return base_type::template is<Type>(); }

	explicit operator bool () const { return has_value(); }

	decltype(auto) value() const { return base_type::template get<Type>(); }
	decltype(auto) value()       { return base_type::template get<Type>(); }

	const remove_reference<Type>* operator -> () const { return &value(); }
	      remove_reference<Type>* operator -> ()       { return &value(); }

	template<typename Handler>
	const optional& if_no_value(Handler&& handler) const {
		if(!has_value()) {
			handler();
		}
		return *this;
	}

	template<typename Handler>
	      optional& if_no_value(Handler&& handler)       {
		if(!has_value()) {
			handler();
		}
		return *this;
	}

	template<typename Handler>
	decltype(auto) if_has_value(Handler&& handler) const {
		if(has_value()) return handler(value());
	}
	template<typename Handler>
	decltype(auto) if_has_value(Handler&& handler)       {
		if(has_value()) return handler(value());
	}

	template<typename Handler>
	optional& set_if_no_value(Handler&& handler) {
		if(!has_value()) {
			*this = handler();
		}
		return *this;
	}

};

template<typename Type>
class optional<Type&> {
	Type* ptr_;
public:

	optional(elements::none = {}) : ptr_{ nullptr } {}

	optional(Type& element) : ptr_{ &element } {}

	bool has_value() const { return ptr_ != nullptr; }

	explicit operator bool () const { return has_value(); }

	decltype(auto) value() const { return *ptr_; }
	decltype(auto) value()       { return *ptr_; }

	const remove_reference<Type>* ptr() const { return ptr_; }
	      remove_reference<Type>* ptr()       { return ptr_; }

	const remove_reference<Type>* operator -> () const { return ptr_; }
	      remove_reference<Type>* operator -> ()       { return ptr_; }

	template<typename Handler>
	const optional& if_no_value(Handler&& handler) const {
		if(!has_value()) {
			handler();
		}
		return *this;
	}
	template<typename Handler>
	      optional& if_no_value(Handler&& handler)       {
		if(!has_value()) {
			handler();
		}
		return *this;
	}

	template<typename Handler>
	decltype(auto) if_has_value(Handler&& handler) const {
		if(has_value()) return handler(value());
	}
	template<typename Handler>
	decltype(auto) if_has_value(Handler&& handler)       {
		if(has_value()) return handler(value());
	}

	template<typename Handler>
	optional& set_if_no_value(Handler&& handler) {
		if(!has_value()) {
			*this = handler();
		}
		return *this;
	}

};