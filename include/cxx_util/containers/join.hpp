#pragma once

#include <algorithm>
#include <iterator>
namespace util {

template<class Container, class It, class Del>
Container join(It begin, It end, const Del& delimiter) {
    Container result{};

    if(std::distance(begin, end) == 0) return result;

    result.insert(result.begin(), *begin);

    std::for_each(begin+1, end, [&](const Container& c) {
        result.insert(result.end(), delimiter);
        result.insert(result.end(), c);
    });

    return result;
}

}