#pragma once

#include <algorithm>
#include <experimental/string_view>

using std::experimental::string_view;

namespace algorithm {

/* Iteratively calculate the Levenshtein edit distance between two strings. */
size_t lev_dist(const string_view &s1, const string_view &s1);

}
