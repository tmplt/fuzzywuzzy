#pragma once

#include "common.hpp"

namespace fuzz {

enum { not_set = -1 };

class string_matcher {
public:
    explicit string_matcher(string_view s1, string_view s2);

    int ratio();
    int partial_ratio();

protected:
    void reset_cache();

    void get_opcodes();

private:
    string_view s1_, s2_;
    int ratio_         = not_set;
    int partial_ratio_ = not_set;
};

}  // ns fuzz
