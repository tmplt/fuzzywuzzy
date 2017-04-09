#pragma once

#include "common.hpp"

extern "C" {
#include "levenshtein.h"
}

using std::vector;

namespace wrapper {

double ratio(const string &str1, const string &str2);

vector<LevMatchingBlock> get_matching_blocks(vector<LevOpCode> &v, string &s1, string &s2);
vector<LevOpCode> get_opcodes(string &s1, string &s2);
vector<LevOpCode> get_opcodes(vector<LevEditOp> &ops, string &s1, string &s2);
vector<LevEditOp> get_editops(string &s1, string &s2);

}  // ns diffutils
