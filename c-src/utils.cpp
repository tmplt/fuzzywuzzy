#include "utils.hpp"
#include "common.hpp"

#include <algorithm>
#include <cmath>

namespace utils {

/*
 * percent-rounded: return an int in the range [0,100].
 * TODO: make up a new name.
 */
int percrnd(double val)
{
    return static_cast<int>(std::round(100 * val));
}

}
