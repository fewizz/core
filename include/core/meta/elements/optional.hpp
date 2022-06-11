#pragma once

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

} // elements

using elements::optional;