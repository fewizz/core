#pragma once

#include "../type/is_same_as.hpp"

namespace wrapper {

	template<typename T, typename = void>
	struct of {
		T m_t;

		/*of() = default;

		template<typename Type>
		requires(type::is_same_as<T>::template for_type_of<Type>)
		of(of<Type>&& t) : m_t{ t.m_t } {};

		template<typename Type>
		requires(type::is_same_as<T>::template for_type_of<Type>)
		of(const of<Type>& t) : m_t{ t.m_t } {};

		template<typename Type>
		requires(type::is_same_as<T>::template for_type_of<Type>)
		of(Type&& t) : m_t{ forward<T>(t) } {}

		template<typename Type>
		requires(type::is_same_as<T>::template for_type_of<Type>)
		of(const Type& t) : m_t{ t } {}*/

		explicit operator const T& () const {
			return m_t;
		}

		explicit operator T& () {
			return m_t;
		}
	};
}