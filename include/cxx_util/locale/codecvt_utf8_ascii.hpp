#pragma once

#include <codecvt>
#include <iterator>
#include <locale>
#include <stdexcept>
#include "../encoding/utf8.hpp"

namespace util {

template<bool Loosing>
class codecvt_utf8_ascii {
	using base_type = std::codecvt<
		char8_t,
		char,
		mbstate_t
	>;
public:

	using state_type = mbstate_t;
	using intern_type = char8_t;
	using extern_type = char;

	std::codecvt_base::result out(
		state_type& state,
		const intern_type* from,
		const intern_type* from_end,
		const intern_type*& from_next,
		extern_type* to,
		extern_type* to_end,
		extern_type*& to_next
	) const {
		if(from > from_end || to > to_end) throw std::runtime_error{"preconditions"};

		auto result = std::codecvt_base::ok;

		while(from < from_end) {
			if(to >= to_end) {
				result = std::codecvt_base::partial;
				break;
			}

			if constexpr(Loosing) {
				auto res = util::utf8::first_codepoint(from, from_end);
				if(result0 != enc::request_result::ok) {
					switch(result0) {
						case enc::request_result::unexpected_src_end :
							result = std::codecvt_base::partial;
						default :
							result = std::codecvt_base::error;
					}
					
					break;
				}

				to[0] = code & 0b01111111;
				from += size;

			}
			else {
				to[0] = from[0];
				++from;
			}

			++to;
		}

		from_next = from;
		to_next = to;

		return result;
	}

	std::codecvt_base::result in(
		state_type& state,
		const extern_type* from,
		const extern_type* from_end,
		const extern_type*& from_next,
		intern_type* to,
		intern_type* to_end,
		intern_type*& to_next
	) const {
		if(from > from_end || to > to_end) throw std::runtime_error{"preconditions"};

		auto result = std::codecvt_base::ok;

		while(from < from_end) {
			if(to >= to_end) {
				result = std::codecvt_base::partial;
				break;
			}
			to[0] = from[0];
			++to;
			++from;
		}

		from_next = from;
		to_next = to;

		return result;
	}

	std::codecvt_base::result unshift(
		state_type& state,
		extern_type* to,
		extern_type* to_end,
		extern_type*& to_next
	) const {
		return std::codecvt_base::ok;
	}

	int encoding() const noexcept {
		return -1;
	}

	bool always_noconv() const noexcept {
		return false;
	}

	int length(
		state_type& state,
		const extern_type* from,   
		const extern_type* from_end,
		std::size_t max
	) const {
		return std::min((size_t)(from_end - from), max);
	}

	int max_length() const noexcept {
		return 4;
	}
};

}