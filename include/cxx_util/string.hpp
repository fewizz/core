#pragma once

#include <iterator>
#include <ranges>
#include <filesystem>
#include <sec_api/stdio_s.h>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>
#include <cstring>

namespace util {

namespace internal {
    template<class D, class...Ts>
    static constexpr bool one_of = (... or std::is_same_v<D, Ts>);

    template<class CharT, class T>
    auto to_string_view(const T& value) {
        using namespace std;
        using D = decay_t<remove_cvref_t<T>>;

        if constexpr (is_same_v<D, basic_string<CharT>>)
            return basic_string_view<CharT>{value.c_str()};
        else
        if constexpr (one_of<D, basic_string_view<CharT>, const CharT*, CharT*>)
            return basic_string_view<CharT>{value};
        else
        if constexpr (is_same_v<D, filesystem::path::string_type>)
            return basic_string_view<CharT>{value.native()};
        else
            return;
    }

    template<class CharT, class T>
    auto to_string(const T& value) {
        using namespace std;
        using D = decay_t<remove_cvref_t<T>>;

        if constexpr (one_of<D, basic_string<CharT>, basic_string_view<CharT>, const CharT*, CharT*>)
            return basic_string<CharT>{value};
        else
        if constexpr(is_same_v<D, filesystem::path>)
            return value.template string<CharT>();
        else
            return;
    }
}

template<class CharT, class T>
static constexpr bool is_convertible_to_string_view_v = 
    std::is_same_v<
        std::remove_cvref_t<decltype( internal::to_string_view<CharT, T>( std::declval<T>() ) )>,
        std::basic_string_view<CharT>
    >; 

template<class T, class CharT>
concept convertible_to_string_view = is_convertible_to_string_view_v<CharT, T>;

template<class CharT, class T>
static constexpr bool is_convertible_to_string_v =
    std::is_same_v<
        std::remove_cvref_t<decltype( internal::to_string<CharT, T>( std::declval<T>() ) )>,
        std::basic_string<CharT>
    >;

template<class T, class CharT>
concept convertible_to_string = is_convertible_to_string_v<CharT, T>;

template<class CharT, convertible_to_string_view<CharT> T>
auto to_string_view(const T& v) {
    return internal::to_string_view<CharT>(v);
}

template<class CharT, convertible_to_string<CharT> T>
auto to_string(const T& v) {
    return internal::to_string<CharT>(v);
}

namespace internal {
    template<class CharT, class T>
    auto to_string_view_or_string(const T& value) {
        if constexpr(is_convertible_to_string_view_v<CharT, T>)
            return to_string_view<CharT>(value);
        else if constexpr(is_convertible_to_string_v<CharT, T>)
            return to_string<CharT>(value);
        else return;
    }
}

template<class CharT, class T>
concept convertible_to_string_view_or_string =
    convertible_to_string_view<CharT, T> or convertible_to_string<CharT, T>;

template<class CharT, convertible_to_string_view_or_string<CharT> T>
auto to_string_view_or_string(const T& value) {
    return internal::to_string_view_or_string<CharT>(value);
}

template<class CharT, convertible_to_string_view_or_string<CharT> T>
typename std::basic_string<CharT>::size_type size(const T& t) {
    using namespace std;
    using D = decay_t<remove_cvref_t<T>>;
    if constexpr(internal::one_of<D, basic_string<CharT>, basic_string_view<CharT>>)
        return t.length();
    else if constexpr(internal::one_of<D, const CharT*, CharT*>)
        return strlen(t);
    else if constexpr(is_same_v<D, filesystem::path>)
        return t.template string<CharT>().length();
}

namespace internal {
    template<class CharT, convertible_to_string_view_or_string<CharT>... Strs>
    inline std::basic_string<CharT> join(
        CharT delimiter,
        std::basic_string<CharT>&& res,
        const convertible_to_string_view_or_string<CharT> auto& str0,
        const Strs&... strs
    ) {
        using namespace std;
        res += delimiter;
        res += to_string_view_or_string<CharT>(str0);
        if constexpr(sizeof...(Strs) == 0)
            return move(res);
        else
            return join<CharT>(delimiter, move(res), strs...);
    }
}

template<class CharT, convertible_to_string_view_or_string<CharT>... Strs>
inline std::basic_string<CharT> join(CharT delimiter, const convertible_to_string_view_or_string<CharT> auto& str0, const Strs&... strs) {
    using namespace std;

    typename basic_string<CharT>::size_type s = (size(str0) + ... + size(strs));
    basic_string<CharT> str;
    str.reserve(s);
    str += to_string_view_or_string<CharT>(str0);

    if constexpr(sizeof...(Strs) == 0)
        return str;
    else
        return internal::join<CharT>(delimiter, move(str), strs...);
}

template<class CharT, std::ranges::range R>
requires convertible_to_string_view_or_string<CharT, std::ranges::range_value_t<R>>
typename std::basic_string<CharT>::size_type size(CharT delimiter, const R& range) {
    typename std::basic_string<CharT>::size_type s = 0;

    for(const auto& it : range) s += size(it);

    s += std::max(std::basic_string<CharT>::size_type(0), std::ranges::size(range) - 1);
    return s;
}

template<class CharT, std::ranges::range R>
requires convertible_to_string_view_or_string<CharT, std::ranges::range_value_t<R>>
inline auto join(CharT delimiter, const R& range) {
    using namespace std;
    std::basic_string<CharT> res;
    res.reserve(size(delimiter, range));

    res += to_string_view_or_string<CharT>(*ranges::begin(range));

    for(const auto& elem : range | std::ranges::views::drop(1)) {
        res += delimiter;
        res += to_string_view_or_string<CharT>(elem);
    }

    return std::move(res);
}

template<class It, class CharT>
concept input_iterator_value_convertible_to_string_view =
    std::input_iterator<It> &&
    convertible_to_string_view<std::iter_value_t<It>, CharT>;

template<class CharT, class T>
static constexpr bool is_assignable_from_string_view_v = std::is_assignable_v<T, std::basic_string_view<CharT>>;

template<class CharT, class T>
static constexpr bool is_constructible_from_string_view_v = std::is_constructible_v<T, std::basic_string_view<CharT>>;

}