#pragma once

namespace u {

template<class T>
concept codec = requires() {
	typename T::encoder_type;
	typename T::decoder_type;
};

}