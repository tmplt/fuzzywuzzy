#pragma once

#include <vector>
#include <experimental/string_view>

#include "structs.hpp"

using std::vector;

namespace diffutils {

double ratio(std::experimental::string_view &s1, std::experimental::string_view &s2);

vector<matching_block> matching_blocks(const vector<op_code> ops, const size_t len1, const size_t len2);

}  // ns diffutils
