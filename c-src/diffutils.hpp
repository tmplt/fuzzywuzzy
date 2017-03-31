#ifndef FUZZYWUZZY_DIFFUTILS_HPP
#define FUZZYWUZZY_DIFFUTILS_HPP

#include "common.hpp"
#include "levenshtein.hpp"

using std::vector;

namespace diffutils {

double ratio(const string &str1, const string &str2);

}  // ns diffutils

#endif //FUZZYWUZZY_DIFFUTILS_HPP
