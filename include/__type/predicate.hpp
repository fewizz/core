#pragma once

struct type_predicate_mark {};

template<typename Type>
concept type_predicate = __is_base_of(type_predicate_mark, Type);