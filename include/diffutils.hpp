#pragma once

#include <vector>
#include <experimental/string_view>

#include "structs.hpp"

using std::vector;
using std::experimental::string_view;

namespace diffutils {

double ratio(sting_view s1, string_view s2);

vector<matching_block> matching_blocks(const vector<op_code> &ops, const size_t len1, const size_t len2);

}  // ns diffutils
