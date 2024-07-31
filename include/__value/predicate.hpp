#pragma once

struct value_predicate_mark {};

template<typename Type>
concept value_predicate = __is_base_of(value_predicate_mark, Type);