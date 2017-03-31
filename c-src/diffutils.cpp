#include <iostream>
#include "diffutils.hpp"
#include "levenshtein.hpp"

namespace diffutils {

double ratio(const string &str1, const string &str2)
{
    size_t len1 = str1.length(),
           len2 = str2.length();

    const lev_byte *lb1 = reinterpret_cast<const lev_byte *>(str1.c_str()),
                   *lb2 = reinterpret_cast<const lev_byte *>(str2.c_str());

    size_t lensum = len1 + len2;
    size_t edit_dist = logarithm::lev_edit_distance(len1, lb1,
                                                    len2, lb2, 1);

    return static_cast<double>(lensum - edit_dist) / lensum;
}

}  // ns diffutils
