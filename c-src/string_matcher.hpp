#pragma once

#include "common.hpp"

namespace fuzz {

enum { not_set = -1 };

class string_matcher {
public:
    explicit string_matcher(string s1, string s2)
        : s1_(s1), s2_(s2) {}

    void set_strings(const string s1, const string s2);
    void set_string1(const string s1);
    void set_string2(const string s2);


    double ratio();
    double real_quick_ratio();

protected:

private:
    string s1_, s2_;
    double ratio_ = not_set;
    int distance_ = not_set;

    void reset_cache();
};

}  // ns fuzz
