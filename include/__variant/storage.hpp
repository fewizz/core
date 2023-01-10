#pragma once

#include "../__type/is_constructible_from.hpp"
#include "../__type/is_reference.hpp"
#include "../__type/remove_reference.hpp"
#include "../__types/first.hpp"
#include "../__types/at_index.hpp"
#include "../integer.hpp"
#include "../if_satisfies.hpp"
#include "../forward.hpp"
#include "../move.hpp"

/*

__variant::storage<typename... Types> - extended union, used in ::variant

	storage() - default constructor

	void create(index, args) - creates object at index with args

	void destroy(index) - destroys object at index

	decltype(auto) view(nuint index, Handler&& handler)

	decltype(auto) get_at<Index>()

*/

namespace __variant {

enum class treat_type_as {
	internal_type, value_type
};

template<typename... Types>
union storage {};

template<typename Type, typename... TailTypes>
union storage<Type, TailTypes...> {

private:
	template<typename... Types0>
	friend union __variant::storage;

	using value_type = Type;

	template<nuint Index>
	using type_at = type_at_index<Index, Type, TailTypes...>;

	static constexpr bool has_next = sizeof...(TailTypes) > 0;
	static constexpr bool value_type_is_reference
		= __type::is_reference<value_type>;

	using next_type = storage<TailTypes...>;

	using internal_type = typename
		if_satisfies<value_type_is_reference>::template
		then<remove_reference<Type> *>::template
		otherwise<Type>;

	template<typename... Args>
	static constexpr bool value_type_is_constructible_from
		= value_type_is_reference ?
			constructible_from<
				internal_type,
				remove_reference<Args>*...
			>
			:
			constructible_from<internal_type, Args...>;

	template<typename... Args>
	static constexpr bool internal_type_is_constructible_from
		= constructible_from<
			internal_type,
			Args...
		>;

	internal_type element_;
	next_type next_;
public:

	constexpr storage() {}

#define CREATE_ELEMENT(storage) \
	if constexpr( \
		TreatAs == treat_type_as::value_type && \
		remove_reference<decltype(storage)>:: \
			value_type_is_reference && \
		remove_reference<decltype(storage)>::template \
			value_type_is_constructible_from<Args...> \
	) { \
		new (&(storage).element_) \
			decltype((storage).element_)(&args...); \
	} \
	else if constexpr( \
		remove_reference<decltype(storage)>::template \
			internal_type_is_constructible_from<Args...> \
	) { \
		new (&(storage).element_) \
			decltype((storage).element_)(forward<Args>(args)...); \
	}

#define CREATE_NEXT(storage) \
	new (&(storage).next_) \
		typename remove_reference<decltype(storage)>::next_type();

#define CREATE_ELEMENT_OR_NEXT(storage, required_index) \
	if(index == required_index) { \
		CREATE_ELEMENT(storage) \
		return; \
	} \
	CREATE_NEXT(storage)

	template<treat_type_as TreatAs, typename... Args>
	constexpr void create(nuint index, Args&&... args) {
		if(index == 0) {
			CREATE_ELEMENT(*this)
			return;
		}
		if constexpr(has_next) {
			CREATE_NEXT(*this)
			next_.template create<TreatAs>(index - 1, forward<Args>(args)...);
			return;
		}
		__builtin_unreachable();
	}

#define DESTROY_ELEMENT(storage) \
	(storage).element_.~decltype((storage).element_)();

#define DESTROY_NEXT(storage) \
	(storage).next_.~decltype((storage).next_)();

#define DESTROY_ELEMENT_OR_NEXT(storage, required_index) \
	if(index == required_index) { \
		DESTROY_ELEMENT(storage) \
		return; \
	} \
	DESTROY_NEXT(storage)

	constexpr ~storage() {}

	// recursive destructor
	constexpr void destroy(nuint index) {
		if(index == 0) {
			DESTROY_ELEMENT(*this)
			return;
		}
		if constexpr(has_next) {
			next_.destroy(index - 1);
			DESTROY_NEXT(*this)
			return;
		}
		__builtin_unreachable();
	}

#define VIEW_ELEMENT(storage) \
	if constexpr( \
		TreatAs == treat_type_as::value_type && \
		remove_reference<decltype(storage)>::value_type_is_reference \
	) { \
		return handler(*(storage).element_); \
	} \
	else { \
		return handler( (storage).element_); \
	}

	template<treat_type_as TreatAs, typename Handler>
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		if(index == 0) {
			VIEW_ELEMENT(*this)
		}
		return next_.template view<TreatAs>(
			index - 1, forward<Handler>(handler)
		);
	}

	template<treat_type_as TreatAs, typename Handler>
	constexpr decltype(auto) view(nuint index, Handler&& handler) const {
		return ((storage&)(*this)).template view<TreatAs>(
			index, [&]<typename E>(E&& element) -> decltype(auto) {
				return handler((const E) element);
			}
		);
	}

	template<nuint Index> requires (Index == 0)
	constexpr const Type& get_at() const & {
		if constexpr (value_type_is_reference) { return *element_; }
		else { return element_; }
	}
	template<nuint Index> requires (Index == 0)
	constexpr       Type& get_at()       & {
		if constexpr (value_type_is_reference) { return *element_; }
		else { return element_; }
	}
	template<nuint Index> requires (Index == 0)
	constexpr const Type&& get_at() const && {
		if constexpr (value_type_is_reference) { return *element_; }
		else { return forward<Type>(element_); }
	}
	template<nuint Index> requires (Index == 0)
	constexpr       Type&& get_at()       && {
		if constexpr (value_type_is_reference) { return *element_; }
		else { return forward<Type>(element_); }
	}

	template<nuint Index> requires (Index > 0 && has_next)
	constexpr const type_at<Index>& get_at() const & {
		return next_.template get_at<Index - 1>();
	}
	template<nuint Index> requires (Index > 0 && has_next)
	constexpr       type_at<Index>& get_at()       & {
		return next_.template get_at<Index - 1>();
	}
	template<nuint Index> requires (Index > 0 && has_next)
	constexpr const type_at<Index>&& get_at() const && {
		return move(*this).next_.template get_at<Index - 1>();
	}
	template<nuint Index> requires (Index > 0 && has_next)
	constexpr       type_at<Index>&& get_at()       && {
		return move(*this).next_.template get_at<Index - 1>();
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 0)
	constexpr void create(nuint, Args&&... args) {
		CREATE_ELEMENT(*this)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 1)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT(next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 2)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT(next_.next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 3)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT_OR_NEXT(next_.next_, 2)
		CREATE_ELEMENT(next_.next_.next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 4)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT_OR_NEXT(next_.next_, 2)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		CREATE_ELEMENT(next_.next_.next_.next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 5)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT_OR_NEXT(next_.next_, 2)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		CREATE_ELEMENT(next_.next_.next_.next_.next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 6)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT_OR_NEXT(next_.next_, 2)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		CREATE_ELEMENT(next_.next_.next_.next_.next_.next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 7)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT_OR_NEXT(next_.next_, 2)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		CREATE_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
	}

	template<treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 8)
	constexpr void create(nuint index, Args&&... args) {
		CREATE_ELEMENT_OR_NEXT(*this, 0)
		CREATE_ELEMENT_OR_NEXT(next_, 1)
		CREATE_ELEMENT_OR_NEXT(next_.next_, 2)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		CREATE_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_.next_, 7)
		CREATE_ELEMENT(next_.next_.next_.next_.next_.next_.next_.next_)
	}

	constexpr void destroy(nuint) requires(sizeof...(TailTypes) == 0) {
		DESTROY_ELEMENT(*this)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 1) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT(next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 2) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT(next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 3) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT(next_.next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 4) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTROY_ELEMENT(next_.next_.next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 5) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTROY_ELEMENT(next_.next_.next_.next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 6) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTROY_ELEMENT(next_.next_.next_.next_.next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 7) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		DESTROY_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 8) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		DESTROY_ELEMENT_OR_NEXT(
			next_.next_.next_.next_.next_.next_.next_, 7)
		DESTROY_ELEMENT(next_.next_.next_.next_.next_.next_.next_.next_)
	}

	constexpr void destroy(nuint index)
	requires(sizeof...(TailTypes) == 9) {
		DESTROY_ELEMENT_OR_NEXT(*this, 0)
		DESTROY_ELEMENT_OR_NEXT(next_, 1)
		DESTROY_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTROY_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		DESTROY_ELEMENT_OR_NEXT(
			next_.next_.next_.next_.next_.next_.next_, 7)
		DESTROY_ELEMENT_OR_NEXT(
			next_.next_.next_.next_.next_.next_.next_.next_, 8)
		DESTROY_ELEMENT(
			next_.next_.next_.next_.next_.next_.next_.next_.next_)
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 0)
	constexpr decltype(auto) view(nuint, Handler&& handler) {
		VIEW_ELEMENT(*this)
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 1)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 2)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 3)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 4)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
			case 4: VIEW_ELEMENT(next_.next_.next_.next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 5)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
			case 4: VIEW_ELEMENT(next_.next_.next_.next_)
			case 5: VIEW_ELEMENT(next_.next_.next_.next_.next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 6)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
			case 4: VIEW_ELEMENT(next_.next_.next_.next_)
			case 5: VIEW_ELEMENT(next_.next_.next_.next_.next_)
			case 6: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 7)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
			case 4: VIEW_ELEMENT(next_.next_.next_.next_)
			case 5: VIEW_ELEMENT(next_.next_.next_.next_.next_)
			case 6: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_)
			case 7: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 8)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
			case 4: VIEW_ELEMENT(next_.next_.next_.next_)
			case 5: VIEW_ELEMENT(next_.next_.next_.next_.next_)
			case 6: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_)
			case 7: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
			case 8: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_.next_.\
				next_)
		}
		__builtin_unreachable();
	}

	template<treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 9)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
			case 3: VIEW_ELEMENT(next_.next_.next_)
			case 4: VIEW_ELEMENT(next_.next_.next_.next_)
			case 5: VIEW_ELEMENT(next_.next_.next_.next_.next_)
			case 6: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_)
			case 7: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
			case 8: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_.next_.\
				next_)
			case 9: VIEW_ELEMENT(next_.next_.next_.next_.next_.next_.next_.\
				next_.next_)
		}
		__builtin_unreachable();
	}

#undef VIEW_ELEMENT

#undef CREATE_ELEMENT
#undef CREATE_NEXT
#undef CREATE_ELEMENT_OR_NEXT

#undef DESTROY_ELEMENT
#undef DESTROY_NEXT
#undef DESTROY_ELEMENT_OR_NEXT

};

}