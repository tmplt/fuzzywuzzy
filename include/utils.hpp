#pragma once

#include <algorithm>  // std::min_element()
#include <vector>     // std::vector

namespace utils {

/*
 * An "extension" of std::min() so that more than two arguments
 * can be passed. The first argument decides What everything else
 * is casted too.
 *
 * Hopefully the compiler will complain if we pass this something stupid.
 */
template <typename T, typename... Args>
constexpr auto min(T first, Args... args)
{
    static_assert(sizeof...(Args) > 1, "use std::min() instead, since it's only two arguments");

    std::vector<T> vec = {first, static_cast<T>(args)...};
    auto min = std::min_element(vec.cbegin(), vec.cend());

    return *min;
}

}
