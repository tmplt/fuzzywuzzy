#ifndef FUZZYWUZZY_DIFFUTILS_HPP
#define FUZZYWUZZY_DIFFUTILS_HPP

#include "common.hpp"
#include "levenshtein.hpp"

using std::vector;

namespace diffutils {

double ratio(const string &str1, const string &str2);

vector<LevMatchingBlock> get_matching_blocks(vector<LevOpCode> &&ops, string &s1, string &s2);
vector<LevOpCode> get_opcodes(string &s1, string &s2);
vector<LevOpCode> get_opcodes(vector<LevEditOp> &ops, string &s1, string &s2);

}  // ns diffutils

#endif //FUZZYWUZZY_DIFFUTILS_HPP
