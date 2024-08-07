#pragma once

#include "../integer.hpp"
#include "../values.hpp"
#include "../__type/remove_reference.hpp"
#include "../__types/are_same.hpp"
#include "../__types/first.hpp"

template<typename Type, nuint Size>
struct array_without_extensions {
	Type storage_[Size];

	constexpr nuint size() const {
		return Size;
	}

	constexpr const Type* iterator() const & { return storage_; }
	constexpr       Type* iterator()       & { return storage_; }
	constexpr const Type* sentinel() const & { return storage_ + Size; }
	constexpr       Type* sentinel()       & { return storage_ + Size; }

	template<typename F>
	constexpr decltype(auto) pass(F&& f) const {
		return [&]<nuint... Indices>(indices::of<Indices...>)
		-> decltype(auto) {
			return f((*this)[Indices]...);
		}(indices::from<0>::to<Size>{});
	}

	constexpr bool operator == (const array_without_extensions& other) const {
		const Type* it_0 = iterator();
		const Type* it_1 = other.iterator();

		for (nuint i = 0; i < Size; ++i) {
			if (*it_0 != *it_1) return false;
		}

		return true;
	}
};

template<typename... Types>
requires(sizeof...(Types) == 1 || types_are_same<Types...>)
array_without_extensions(Types&&...)
	-> array_without_extensions<
		remove_reference<first_type<Types...>>,
		sizeof...(Types)
	>;