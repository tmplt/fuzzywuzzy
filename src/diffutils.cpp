#include "diffutils.hpp"
#include "algorithm.hpp"

namespace diffutils {

double ratio(string_view s1, string_view s2)
{
    size_t len1 = s1.length(), len2 = s2.length();
    size_t lensum = len1 + len2;

    size_t edit_dist = algorithm::lev_dist(s1, s2);

    return (lensum - edit_dist) / static_cast<double>(lensum);
}

}  // ns diffutils
