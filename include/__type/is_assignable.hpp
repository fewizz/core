#pragma once

template<typename Type, typename WhatToAssign>
concept assignable = __is_assignable(Type, WhatToAssign);