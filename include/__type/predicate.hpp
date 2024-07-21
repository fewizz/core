#pragma once

struct type_predicate_marker {};

template<typename Type>
concept type_predicate = __is_base_of(type_predicate_marker, Type);