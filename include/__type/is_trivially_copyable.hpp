#pragma once

template<typename Type>
concept trivially_copyable = __is_trivially_copyable(Type);