#pragma once

#include "encoding.hpp"
#include "../locale/facet.hpp"
#include "../locale/codecvt.hpp"
#include "../locale/codecvt_utf8_ascii.hpp"
#include "../locale/codecvt_utf16_ascii.hpp"
#include <codecvt>
#include <cstring>

namespace enc {

template<enc::encoding Internal, enc::encoding External>
struct codec;

// utf8
template<> struct codec<enc::utf8, enc::utf8> {
    constexpr static bool nonconv = true;
};

// utf16
template<> struct codec<enc::utf16, enc::utf16> {
    constexpr static bool nonconv = true;
};

// ascii
template<> struct codec<enc::ascii, enc::ascii> {
    constexpr static bool nonconv = true;
};

// utf8 <-> utf16
template<> struct codec<enc::utf16, enc::utf8> {
    using type = std::codecvt_utf8_utf16<char16_t>;//util::deletable_facet<std::codecvt<char16_t, char8_t, std::mbstate_t>>;
    constexpr static bool reverted = false;
    constexpr static bool nonconv = false;
};

template<> struct codec<enc::utf8, enc::utf16> {
    using type = util::deletable_facet<std::codecvt<char16_t, char8_t, std::mbstate_t>>;
    constexpr static bool reverted = true;
    constexpr static bool nonconv = false;
};

// utf8 <-> ascii
template<> struct codec<enc::utf8, enc::ascii> {
    using type = util::codecvt_utf8_ascii<false>;
    constexpr static bool reverted = false;
    constexpr static bool nonconv = false;
};

template<> struct codec<enc::ascii, enc::utf8> {
    using type = util::codecvt_utf8_ascii<false>;
    constexpr static bool reverted = true;
    constexpr static bool nonconv = false;
};

// utf16 <-> ascii
template<> struct codec<enc::utf16, enc::ascii> {
    using type = util::codecvt_utf16_ascii<false>;
    constexpr static bool reverted = false;
    constexpr static bool nonconv = false;
};

template<> struct codec<enc::ascii, enc::utf16> {
    using type = util::codecvt_utf16_ascii<false>;
    constexpr static bool reverted = true;
    constexpr static bool nonconv = false;
};

//

namespace internal {

template <enc::encoding Encoding>
struct to_chain {
    using from_type = typename Encoding::char_type;

    const from_type* from_begin;
    const from_type* from_end;

    template<enc::encoding Encoding0>
    bool to_always_noconv() {
        using codec_t = codec<Encoding, Encoding0>;

        if constexpr(codec_t::nonconv)
            return true;
        else {
            typename codec_t::type c{};
            return c.always_noconv();
        }
    }

    template<enc::encoding Encoding0>
    std::codecvt_base::result to(
        typename Encoding0::char_type* begin,
        typename Encoding0::char_type* end,
        typename Encoding::char_type*& from_next,
        typename Encoding0::char_type*& to_next
    ) {
        using codec_t = codec<Encoding, Encoding0>;

        if constexpr(codec_t::nonconv) {
            return std::codecvt_base::noconv;
        }
        else {
            typename codec_t::type c{};
            typename codec_t::type::state_type st{};

            if constexpr(codec_t::reverted)
                return c.in(st, from_begin, from_end, from_next, begin, end, to_next);
            else
                return c.out(st, from_begin, from_end, from_next, begin, end, to_next);
        }
    }

    template<enc::encoding Encoding0>
    void to(
        typename Encoding0::char_type* begin,
        typename Encoding0::char_type* end
    ) {
        using codec_t = codec<Encoding, Encoding0>;

        if constexpr(codec_t::nonconv) {
            int from_size = from_end - from_begin;
            int to_size = end - begin;

            if(to_size < from_size ) throw std::runtime_error{"not enought space"};

            std::memcpy(begin, from_begin, from_size);
        }
        else {
            typename codec_t::type c{};
            typename codec_t::type::state_type st{};

            
            using in_t = typename codec_t::type::intern_type;
            using ex_t = typename codec_t::type::extern_type;

            if constexpr(codec_t::reverted) {
                const ex_t* from_next;
                in_t* to_next;

                c.in(
                    st,
                    (const ex_t*)from_begin,
                    (const ex_t*)from_end,
                    from_next,
                    (in_t*)begin,
                    (in_t*)end,
                    to_next
                );
            }
            else {
                const in_t* from_next;
                ex_t* to_next;

                c.out(
                    st,
                    (const in_t*)from_begin,
                    (const in_t*)from_end,
                    from_next,
                    (ex_t*)begin,
                    (ex_t*)end,
                    to_next
                );
            }
        }
    }

    template<enc::encoding Encoding0>
    int to_length() {
        using codec_t = codec<Encoding, Encoding0>;

        if constexpr(codec_t::nonconv) {
            return from_end - from_begin;
        }
        else {
            if constexpr(codec_t::reverted)
                return util::codecvt<typename codec_t::type>::from_external_to_internal_length(from_begin, from_end); 
            else
                return util::codecvt<typename codec_t::type>::from_internal_to_external_length(from_begin, from_end);
        }

    }
};

}

template<enc::encoding Encoding>
auto from(
    const typename Encoding::char_type* begin,
    const typename Encoding::char_type* end
) {
    return enc::internal::to_chain<Encoding>{begin, end};
}

template<enc::encoding Encoding, unsigned N>
auto from(const typename Encoding::char_type (& arr)[N]) {
    return enc::internal::to_chain<Encoding>{arr, arr + N};
}

}