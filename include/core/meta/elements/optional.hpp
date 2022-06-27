#pragma once

#include "none.hpp"
#include "one_of.hpp"

namespace elements {

	template<typename Type>
	class optional : public elements::one_of<Type, elements::none> {
		using base_type = elements::one_of<Type, elements::none>;
	public:

		using base_type::base_type;

		optional() : base_type{ elements::none{} } {}

		bool has_value() const {
			return base_type::template is<Type>();
		}

		explicit operator bool () const {
			return has_value();
		}

		decltype(auto) value() const {
			return base_type::template get<Type>();
		}

		decltype(auto) value() {
			return base_type::template get<Type>();
		}

		const remove_reference<Type>* operator -> () const {
			return &value();
		}

		remove_reference<Type>* operator -> () {
			return &value();
		}

	};

	template<typename Type>
	class optional<Type&> {
		Type* ptr_;
	public:

		optional(elements::none = {}) : ptr_{ nullptr } {}

		optional(Type& element) : ptr_{ &element } {}

		bool has_value() const {
			return ptr_ != nullptr;
		}

		explicit operator bool () const {
			return has_value();
		}

		decltype(auto) value() const {
			return *ptr_;
		}

		decltype(auto) value() {
			return *ptr_;
		}

		const remove_reference<Type>* ptr() const {
			return ptr_;
		}

		remove_reference<Type>* ptr() {
			return ptr_;
		}

		const remove_reference<Type>* operator -> () const {
			return ptr_;
		}

		remove_reference<Type>* operator -> () {
			return ptr_;
		}

	};

} // elements

template<typename Type>
using optional = elements::optional<Type>;