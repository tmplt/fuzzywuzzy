#pragma once

#include <vector>
#include <experimental/string_view>

#include "structs.hpp"

using std::vector;
using std::experimental::string_view;

namespace diffutils {

vector<edit_op> get_edit_ops(string_view s1, string_view s2);

vector<edit_op> edit_ops_from_cost_matrix(size_t len1, string_view s1, size_t p1, size_t o1,
                                           size_t len2, string_view s2, size_t p2, size_t o2,
                                           std::vector<size_t> &matrix);

vector<op_code> edit_ops_to_op_codes(vector<edit_op> &ops, size_t len1, size_t len2);

vector<matching_block> get_matching_blocks(size_t len1, size_t len2, vector<op_code> &ops);
vector<matching_block> get_matching_blocks(size_t len1, size_t len2, vector<edit_op> &ops);
vector<matching_block> get_matching_blocks(string_view &s1, string_view &s2);

}  // ns diffutils
