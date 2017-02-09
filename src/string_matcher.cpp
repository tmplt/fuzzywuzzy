#include "string_matcher.hpp"
#include "algorithm.hpp"
#include "diffutils.hpp"

#include <algorithm>
#include <cassert>

namespace fuzz {

void string_matcher::set_strings(const string_view s1, const string_view s2)
{
    s1_ = s1;
    s2_ = s2;

    reset_cache();
}

void string_matcher::set_string1(const string_view s1)
{
    s1_ = s1;
    reset_cache();
}

void string_matcher::set_string2(const string_view s2)
{
    s2_ = s2;
    reset_cache();
}

vector<matching_block> string_matcher::get_matching_blocks()
{
    if (matching_blocks_.empty())
        matching_blocks_ = diffutils::matching_blocks(get_opcodes(), s1_.length(), s2_.length());
    return matching_blocks_;
}

double string_matcher::ratio()
{
    if (!ratio_)
        ratio_ = diffutils::ratio(s1_, s2_);
    return ratio_;
}

double string_matcher::real_quick_ratio()
{
    size_t len1 = s1_.length(), len2 = s2_.length();
    return 2.0 * std::min(len1, len2) / (len1 + len2);
}

int string_matcher::distance()
{
    if (!distance_)
        distance_ = algorithm::levenshtein_distance(s1_, s2_);
    return distance_;
}

void string_matcher::reset_cache()
{
    ratio_ = distance_ = not_set;

    matching_blocks_.clear();
    op_codes_.clear();
}

vector<op_code> string_matcher::get_opcodes()
{
    if (op_codes_.empty())
        op_codes_ = diffutils::opcodes(s1_, s2_);
    return op_codes_;
}

}  // ns fuzz
