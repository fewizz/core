#pragma once

namespace u {

template<class T>
concept character_set = requires {
	typename T::codepoint_base_type;
};

}