#pragma once

#include <compare>
#include <type_traits>
#include <utility>
#include <span>
#include "../encoding/encoding.hpp"
//#include "../encoding/convert.hpp"
#include "../int.hpp"

namespace vw {

/*template<
	enc::encoding Encoding,
	class T = util::uint_with_size_t<Encoding::preferred_size>
>
struct character : std::vector<T> {
	using value_type = T;
	using base_type = std::vector<T>;

	character(const character& ch) {};
	character(character&& base) {};

	using base_type::data;
	using base_type::size;

	auto operator <=> (const character& other) const {
		return
			Encoding::first_codepoint(data(), data() + size()).codepoint
			<=>
			Encoding::first_codepoint(other.data(), other.data() + other.size()).codepoint
		;
	}

		bool operator == (auto ch) const requires(std::is_integral_v<decltype(ch)>) {
			return
				this->size() == 1
				&&
				Encoding::first_codepoint(data(), data() + size()).codepoint == ch
			;
		}

		template<enc::encoding E>
		vw::character<E> convert() const {
			auto from = enc::template from<Encoding>(data(), data() + size());

			if(from.template to_always_noconv<E>()) {
				return {data(), data() + size()};
			}

			std::basic_string<typename E::char_type> str;
			str.resize(
				from.template to_length<E>()
			);

			from.template to<E>(
				str.data(), str.data() + str.size()
			);

			return {std::move(str)};
		}

		template<enc::encoding E>
		vw::character<E> to_string() const {
			return convert<E>();
		}

		template<class CharT>
		requires(sizeof(char_type) == sizeof(CharT) && std::is_integral_v<CharT>)
		std::basic_string<CharT> to_string() const {
			return { (CharT*)Base::data(), Base::size() };
		}

		template<enc::encoding E, class CharT>
		std::basic_string<CharT> to_string() const {
			return to_string<E>().template to_string<CharT>();
		}

		auto to_string_view() const {
			return to_string_view<char_type>();
		}

		template<enc::encoding E = Encoding>
		operator std::enable_if_t<E::characters != enc::variable_width, char_type> () const {
			return Base::front();
		}
	};
//}*/

/*template<enc::encoding Encoding>
struct character_view : internal::character<std::basic_string_view<typename Encoding::char_type>, Encoding> {
	using internal::character<std::basic_string_view<typename Encoding::char_type>, Encoding>::character;
};

template<enc::encoding Encoding>
struct character : internal::character<std::basic_string<typename Encoding::char_type>, Encoding> {
	using char_type = typename Encoding::char_type;
	using string_type = std::basic_string<char_type>;
	using base_type = internal::character<string_type, Encoding>;

	using base_type::base_type;

	operator character_view<Encoding> () const {
		return { string_type::operator std::template basic_string_view<char_type>() };
	}
};*/

template<
	enc::encoding Encoding,
	class T
>
struct character_view : std::span<T> {
	using base_type = std::vector<T>;

	character_view() = default;
	character_view(const character_view& ch) = default;
	character_view(character_view&& base) = default;

	using base_type::data;
	using base_type::size;

	auto operator <=> (const character_view& that) const {
		return
			enc::codepoint<Encoding>(*this)
			<=>
			enc::codepoint<Encoding>(that)
		;
	}
};

}