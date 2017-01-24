#pragma once

#include "structs.hpp"
#include "common.hpp"

namespace fuzz {

enum { not_set = -1 };

class string_matcher {
public:
    explicit string_matcher(string_view s1, string_view s2)
        : s1_(s1), s2_(s2) {}

    vector<matching_block> get_matching_blocks(string_view &s1, string_view &s2);

    void set_strings(string_view s1, string_view s2);
    void set_string1(string_view s1);
    void set_string2(string_view s2);

    vector<op_code> get_opcodes();
    vector<edit_op> get_edit_ops();

    vector<matching_block> get_matching_blocks();

    double ratio();
    int real_quick_ratio();
    int distance();

protected:
    vector<matching_block> get_matching_blocks(int len1, int len2, vector<op_code> ops);
    vector<matching_block> get_matching_blocks(int len1, int len2, vector<edit_op> ops);

    vector<edit_op> get_edit_ops(string_view &s1, string_view &s2);
    vector<edit_op> get_edit_ops_from_cost_matrix(int len1, string_view s1, int p1, int o1,
                                                  int len2, string_view s2, int p2, int o2,
                                                  vector<int> matrix);

    void reset_cache();

    void get_opcodes();

private:
    string_view s1_, s2_;
    int ratio_    = not_set;
    int distance_ = not_set;

    vector<matching_block> matching_blocks_;
    vector<edit_op> edit_ops_;
    vector<op_code> op_codes_;
};

}  // ns fuzz
