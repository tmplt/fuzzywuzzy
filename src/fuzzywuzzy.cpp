#include <iostream>
#include <set>
#include "fuzzywuzzy.hpp"
#include "string_matcher.hpp"
#include "utils.hpp"

namespace fuzz {

int ratio(const string &s1, const string &s2)
{
    auto m = string_matcher(s1, s2);
    return utils::percent_round(m.ratio());
}

int partial_ratio(const string &s1, const string &s2)
{
    string shorter, longer;

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
        size_t long_start = (block.dpos - block.spos) > 0 ? block.dpos - block.spos : 0;
        size_t long_end   = long_start + shorter.length();

        auto long_substr = longer.substr(long_start, long_end);
        auto m2 = string_matcher(shorter, long_substr);
        double r = m2.ratio();

        if (r > 0.995)
            return 100;
        else
            scores.push_back(r);
    }

    if (scores.empty())
        return 0;

    double max = *std::max_element(scores.cbegin(), scores.cend());
    return utils::percent_round(max);
}

/* Returns a cleaned string with tokens sorted. */
static string proccess_and_sort(const string &s, bool full_process)
{
    string ps = (full_process ? utils::full_process(s) : s);

    auto tokens = utils::split_string(ps);

    std::sort(tokens.begin(), tokens.end());
    string sorted = "";
    for (auto token : tokens)
        sorted += token + (token != tokens.back() ? " " : "");

    return utils::trim(sorted);
}

int token_sort_ratio(const string &s1, const string &s2, bool full_proccess)
{
    /* NOTE: do we need force_ascii? */
    string sorted1 = proccess_and_sort(s1, full_proccess);
    string sorted2 = proccess_and_sort(s2, full_proccess);

    return ratio(sorted1, sorted2);
}

int token_sort_partial_ratio(const string &s1, const string &s2, bool full_proccess)
{
    /* NOTE: do we need force_ascii? */
    string sorted1 = proccess_and_sort(s1, full_proccess);
    string sorted2 = proccess_and_sort(s2, full_proccess);

    return partial_ratio(sorted1, sorted2);
}

/*
 * Find all alphanumeric tokens in each string and:
 *  - treat them as a set,
 *  - construct two strings of the form <sorted_intersection><sorted_remainder>,
 *  - take ratios of those two strings, and
 *  - check for unordered partial matches.
 */
static int token_set_ratio(const string &s1, const string &s2, bool partial, bool full_process)
{
    string p1 = full_process ? utils::full_process(s1) : s1;
    string p2 = full_process ? utils::full_process(s2) : s2;

    if (p1.length() == 0 || p2.length() == 0)
        return 0;

    /* This next part is a wee bit easier on the eyes in Python. */

    auto split1 = utils::split_string(p1), split2 = utils::split_string(p2);
    auto tokens1 = std::set<string>(split1.cbegin(), split1.cend()),
         tokens2 = std::set<string>(split2.cbegin(), split2.cend());

    vector<string> intersection, diff1to2, diff2to1;

    std::set_intersection(tokens1.cbegin(), tokens1.cend(),
                          tokens2.cbegin(), tokens2.cend(),
                          std::back_inserter(intersection));

    std::set_difference(tokens1.cbegin(), tokens1.cend(),
                        tokens2.cbegin(), tokens2.cend(),
                        std::back_inserter(diff1to2));
    std::set_difference(tokens2.cbegin(), tokens2.cend(),
                        tokens1.cbegin(), tokens1.cend(),
                        std::back_inserter(diff2to1));

    std::sort(intersection.begin(), intersection.end());
    std::sort(diff1to2.begin(), diff1to2.end());
    std::sort(diff2to1.begin(), diff2to1.end());

    auto sorted_sect = utils::join(intersection),
         sorted_1to2 = utils::join(diff1to2),
         sorted_2to1 = utils::join(diff2to1);

    auto combined_1to2 = sorted_sect + " " + sorted_1to2,
         combined_2to1 = sorted_sect + " " + sorted_2to1;

    sorted_sect = utils::trim(sorted_sect);
    combined_1to2 = utils::trim(combined_1to2);
    combined_2to1 = utils::trim(combined_2to1);

    auto ratio_func = partial ? partial_ratio : ratio;
    auto pairwise = vector<int>{
        ratio_func(sorted_sect, combined_1to2),
        ratio_func(sorted_sect, combined_2to1),
        ratio_func(combined_1to2, combined_2to1)
    };

    return *std::max_element(pairwise.cbegin(), pairwise.cend());
}

int token_set_ratio(const string &s1, const string &s2, bool full_process)
{
    return token_set_ratio(s1, s2, false, full_process);
}

int partial_token_set_ratio(const string &s1, const string &s2, bool full_process)
{
    return token_set_ratio(s1, s2, true, full_process);
}

}  // ns fuzz
