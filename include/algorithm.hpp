#pragma once

#include <algorithm>
#include <experimental/string_view>

using std::experimental::string_view;

namespace algorithm {

/* Iteratively calculate the Levenshtein edit distance between two strings. */
size_t levenshtein_distance(const string_view &s1, const string_view &s1);

}
