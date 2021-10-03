#pragma once

template<typename Type>
concept enum_type = __is_enum(Type);

namespace type {
	struct is_enum {
		static constexpr bool is_types_predicate = true;

		template<typename Type>
		static constexpr bool for_type_of = enum_type<Type>;
	};
}