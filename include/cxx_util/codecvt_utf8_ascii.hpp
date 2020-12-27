#pragma once

#include <codecvt>
#include "utf8.hpp"

namespace util {

template<bool Loosing>
struct codecvt_utf8_ascii {
    using base_type = std::codecvt<
        char8_t,
        char,
        mbstate_t
    >;

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
        while(from < from_end) {
            if constexpr(Loosing) {
                auto [code, size] = util::utf8::first_code_point(from, from_end);
                to[0] = code & 0b01111111;
                from += size;
                ++to;
            }
            else {
                to[0] = from[0];
                ++from;
                ++to;
            }
        }

        from_next = from;
        to_next = to;

        return std::codecvt_base::ok;
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
        while(from < from_end) {
            to[0] = from[0];
            ++to;
            ++from;
        }

        from_next = from;
        to_next = to;

        return std::codecvt_base::ok;
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