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
 * NOTE: Can this be done when omitting first?
 */
template <typename T, typename... Args>
auto min(const T &first, const Args&... args)
{
    /* sizeof is one less than it should be since the first argument isn't counted. */
    static_assert(sizeof...(Args) >= 2, "use std::min() instead, since it's only two arguments");

    std::vector<T> vec = {first, static_cast<T>(args)...};
    auto min = std::min_element(vec.cbegin(), vec.cend());

    return *min;
}

/*
 * percent-rounded: return an int in the range [0,100].
 * TODO: make up a new name.
 */
int percrnd(double val);

}
