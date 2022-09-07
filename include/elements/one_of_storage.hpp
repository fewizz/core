#pragma once

#include "../__type/is_constructible_from.hpp"
#include "../__type/is_reference.hpp"
#include "../__type/remove_reference.hpp"
#include "../__types/first.hpp"
#include "../integer.hpp"
#include "../if_satisfies.hpp"
#include "../forward.hpp"

namespace elements {

enum class one_of_storage_treat_type_as {
	internal_type, value_type
};

template<typename... Types>
union one_of_storage {};

template<typename Type, typename... TailTypes>
union one_of_storage<Type, TailTypes...> {
private:

	template<typename... Types0>
	friend union one_of_storage;

	using value_type = Type;

	static constexpr bool has_next = sizeof...(TailTypes) > 0;
	static constexpr bool value_type_is_reference
		= __type::is_reference<value_type>;

	using next_type = one_of_storage<TailTypes...>;

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
public:

	internal_type element_;
	next_type next_;

	constexpr one_of_storage() {}

#define INIT_ELEMENT(storage) \
	if constexpr( \
		TreatAs == one_of_storage_treat_type_as::value_type && \
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

#define INIT_NEXT(storage) \
	new (&(storage).next_) \
		typename remove_reference<decltype(storage)>::next_type();

#define INIT_ELEMENT_OR_NEXT(storage, required_index) \
	if(index == required_index) { \
		INIT_ELEMENT(storage) \
		return; \
	} \
	INIT_NEXT(storage)

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	constexpr void init(nuint index, Args&&... args) {
		if(index == 0) {
			INIT_ELEMENT(*this)
			return;
		}
		if constexpr(has_next) {
			INIT_NEXT(*this)
			next_.template init<TreatAs>(index - 1, forward<Args>(args)...);
			return;
		}
		__builtin_unreachable();
	}

#define DESTRUCT_ELEMENT(storage) \
	(storage).element_.~decltype((storage).element_)();

#define DESTRUCT_NEXT(storage) \
	(storage).next_.~decltype((storage).next_)();

#define DESTRUCT_ELEMENT_OR_NEXT(storage, required_index) \
	if(index == required_index) { \
		DESTRUCT_ELEMENT(storage) \
		return; \
	} \
	DESTRUCT_NEXT(storage)

	constexpr ~one_of_storage() {}

	// trivial recursive destructor
	constexpr void destruct(nuint index) {
		if(index == 0) {
			DESTRUCT_ELEMENT(*this)
			return;
		}
		if constexpr(has_next) {
			next_.destruct(index - 1);
			DESTRUCT_NEXT(*this)
			return;
		}
		__builtin_unreachable();
	}

#define VIEW_ELEMENT(storage) \
	if constexpr( \
		TreatAs == one_of_storage_treat_type_as::value_type && \
		remove_reference<decltype(storage)>::value_type_is_reference \
	) { \
		return handler(*(storage).element_); \
	} \
	else { \
		return handler( (storage).element_); \
	}

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		if(index == 0) {
			VIEW_ELEMENT(*this)
		}
		return next_.template view<TreatAs>(
			index - 1, forward<Handler>(handler)
		);
	}

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
	constexpr decltype(auto) view(nuint index, Handler&& handler) const {
		return ((one_of_storage&)(*this)).template view<TreatAs>(
			index, [&]<typename E>(E&& element) -> decltype(auto) {
				return handler((const E) element);
			}
		);
	}

	template<nuint Index> requires (Index == 0)
	constexpr const auto& at() const {
		if constexpr (value_type_is_reference) {
			return *element_;
		}
		else {
			return element_;
		}
	}

	template<nuint Index> requires (Index == 0)
	constexpr auto& at() {
		if constexpr (value_type_is_reference) {
			return *element_;
		}
		else {
			return element_;
		}
	}

	template<nuint Index> requires (Index > 0 && has_next)
	constexpr const auto& at() const {
		return next_.template at<Index - 1>();
	}

	template<nuint Index> requires (Index > 0 && has_next)
	constexpr auto& at() {
		return next_.template at<Index - 1>();
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 0)
	constexpr void init(nuint, Args&&... args) {
		INIT_ELEMENT(*this)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 1)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT(next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 2)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT(next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 3)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT_OR_NEXT(next_.next_, 2)
		INIT_ELEMENT(next_.next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 4)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT_OR_NEXT(next_.next_, 2)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		INIT_ELEMENT(next_.next_.next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 5)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT_OR_NEXT(next_.next_, 2)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		INIT_ELEMENT(next_.next_.next_.next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 6)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT_OR_NEXT(next_.next_, 2)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		INIT_ELEMENT(next_.next_.next_.next_.next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 7)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT_OR_NEXT(next_.next_, 2)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		INIT_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename... Args>
	requires(sizeof...(TailTypes) == 8)
	constexpr void init(nuint index, Args&&... args) {
		INIT_ELEMENT_OR_NEXT(*this, 0)
		INIT_ELEMENT_OR_NEXT(next_, 1)
		INIT_ELEMENT_OR_NEXT(next_.next_, 2)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		INIT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_.next_, 7)
		INIT_ELEMENT(next_.next_.next_.next_.next_.next_.next_.next_)
	}

	constexpr void destruct(nuint) requires(sizeof...(TailTypes) == 0) {
		DESTRUCT_ELEMENT(*this)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 1) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT(next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 2) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT(next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 3) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT(next_.next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 4) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTRUCT_ELEMENT(next_.next_.next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 5) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTRUCT_ELEMENT(next_.next_.next_.next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 6) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTRUCT_ELEMENT(next_.next_.next_.next_.next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 7) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		DESTRUCT_ELEMENT(next_.next_.next_.next_.next_.next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 8) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		DESTRUCT_ELEMENT_OR_NEXT(
			next_.next_.next_.next_.next_.next_.next_, 7)
		DESTRUCT_ELEMENT(next_.next_.next_.next_.next_.next_.next_.next_)
	}

	constexpr void destruct(nuint index)
	requires(sizeof...(TailTypes) == 9) {
		DESTRUCT_ELEMENT_OR_NEXT(*this, 0)
		DESTRUCT_ELEMENT_OR_NEXT(next_, 1)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_, 2)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_, 3)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_, 4)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_, 5)
		DESTRUCT_ELEMENT_OR_NEXT(next_.next_.next_.next_.next_.next_, 6)
		DESTRUCT_ELEMENT_OR_NEXT(
			next_.next_.next_.next_.next_.next_.next_, 7)
		DESTRUCT_ELEMENT_OR_NEXT(
			next_.next_.next_.next_.next_.next_.next_.next_, 8)
		DESTRUCT_ELEMENT(
			next_.next_.next_.next_.next_.next_.next_.next_.next_)
	}

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 0)
	constexpr decltype(auto) view(nuint, Handler&& handler) {
		VIEW_ELEMENT(*this)
	}

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 1)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
		}
		__builtin_unreachable();
	}

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
	requires (sizeof...(TailTypes) == 2)
	constexpr decltype(auto) view(nuint index, Handler&& handler) {
		switch (index) {
			case 0: VIEW_ELEMENT(*this)
			case 1: VIEW_ELEMENT(next_)
			case 2: VIEW_ELEMENT(next_.next_)
		}
		__builtin_unreachable();
	}

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

	template<one_of_storage_treat_type_as TreatAs, typename Handler>
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

#undef INIT_ELEMENT
#undef INIT_NEXT
#undef INIT_ELEMENT_OR_NEXT

#undef DESTRUCT_ELEMENT
#undef DESTRUCT_NEXT
#undef DESTRUCT_ELEMENT_OR_NEXT

};

} // elements