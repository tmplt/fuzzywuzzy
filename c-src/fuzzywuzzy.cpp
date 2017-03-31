#include "fuzzywuzzy.hpp"
#include "string_matcher.hpp"
#include "utils.hpp"

namespace fuzz {

int ratio(const string &s1, const string &s2)
{
    auto m = string_matcher(s1, s2);
    return utils::percrnd(m.ratio());
}

}  // ns fuzz
