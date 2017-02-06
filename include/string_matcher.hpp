#pragma once

#include "common.hpp"
#include "structs.hpp"

namespace fuzz {

enum { not_set = -1 };

class string_matcher {
public:
    explicit string_matcher(string_view s1, string_view s2)
        : s1_(s1), s2_(s2) {}

    void set_strings(const string_view s1, const string_view s2);
    void set_string1(const string_view s1);
    void set_string2(const string_view s2);

    vector<matching_block> get_matching_blocks();

    double ratio();
    double real_quick_ratio();
    int distance();

protected:

private:
    string_view s1_, s2_;
    int ratio_    = not_set;
    int distance_ = not_set;

    void reset_cache();
    vector<op_code> get_opcodes();

    vector<matching_block> matching_blocks_;
    vector<op_code> op_codes_;
};

}  // ns fuzz
