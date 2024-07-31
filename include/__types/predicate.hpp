#pragma once
	
struct types_predicate_mark {};

template<typename Type>
concept types_predicate = __is_base_of(types_predicate_mark, Type);