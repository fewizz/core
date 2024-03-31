#pragma once

#include "./copy_cv.hpp"
#include "./copy_ref.hpp"


namespace __type {

	template<typename From>
	struct copy_const_ref_t {
		template<typename To>
		using type = To;
	};

	template<typename From>
	struct copy_const_ref_t<From&> {
		template<typename To>
		using type = To&;
	};

	template<typename From>
	struct copy_const_ref_t<From&&> {
		template<typename To>
		using type = To&&;
	};

	template<typename From>
	struct copy_const_ref_t<const From> {
		template<typename To>
		using type = const To;
	};

	template<typename From>
	struct copy_const_ref_t<const From&> {
		template<typename To>
		using type = const To&;
	};

	template<typename From>
	struct copy_const_ref_t<const From&&> {
		template<typename To>
		using type = const To&&;
	};

}


template<typename From, typename To>
using copy_const_ref = __type::copy_const_ref_t<From>::template type<To>;