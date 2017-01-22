#pragma once

#include <vector>
#include <experimental/string_view>

#include "structs.hpp"

using std::vector;
using std::experimental::string_view;

namespace diffutils {

std::vector<edit_op> get_edit_ops(string_view s1, string_view s2);

std::vector<edit_op> edit_opts_from_cost_matrix(size_t len1, string_view s1, size_t p1, size_t o1,
                                                size_t len2, string_view s2, size_t p2, size_t o2,
                                                std::vector<size_t> &matrix);

std::vector<matching_block> get_matching_blocks(size_t len1, size_t len2);

}  // ns diffutils
