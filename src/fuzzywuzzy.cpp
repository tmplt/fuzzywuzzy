#include "fuzzywuzzy.hpp"
#include "string_matcher.hpp"
#include "utils.hpp"

#include <cmath>
#include <algorithm>

namespace fuzz {

int ratio(const string_view &s1, const string_view &s2)
{
    auto m = string_matcher(s1, s2);
    return static_cast<int>(std::round(100 * m.ratio()));
}

int partial_ratio(string_view &s1, string_view &s2)
{
    string_view shorter, longer;

    if (s1.length() <= s2.length()) {
        shorter = s1;
        longer  = s2;
    } else {
        shorter = s2;
        longer  = s1;
    }

    auto m = string_matcher(shorter, longer);
    auto blocks = m.get_matching_blocks();

    /*
     * Each block represents a string of matching characters
     * in a string of the form (idx_1, idx_2, len). The best
     * partial match will block align with at least one
     * of those blocks.
     *  e.g. shorter = "abcd", longer "XXXbcdeEEE"
     *  block = (1, 3, 3)
     *  best score == ratio("abcd", "Xbcd")
     */
    vector<double> scores;
    for (const auto &block : blocks) {
        size_t long_start = (block.dpos - block.spos) > 0 ? block.dpos - block.spos : 0,
               long_end   = long_start + shorter.length();

        auto long_substr = longer.substr(long_start, long_end);
        auto m2 = string_matcher(shorter, long_substr);
        int r = m2.ratio();

        if (r > 0.995)
            return 100;
        else
            scores.push_back(r);
    }

    double max = *std::max_element(scores.cbegin(), scores.cend());
    return std::round(100 * std::move(max));
}

int token_sort_ratio(string_view &s1, string_view &s2)
{
    //auto sorted1 = utils::sort(s1);

    return 666;
}

}  // ns fuzz
