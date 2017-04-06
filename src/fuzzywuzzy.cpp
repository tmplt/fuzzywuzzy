#include <iostream>
#include "fuzzywuzzy.hpp"
#include "string_matcher.hpp"
#include "utils.hpp"

namespace fuzz {

int ratio(const string &s1, const string &s2)
{
    auto m = string_matcher(s1, s2);
    return utils::percrnd(m.ratio());
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
    return utils::percrnd(max);
}

/* Returns a cleaned string with tokens sorted. */
static string proccess_and_sort(string s, bool full_process)
{
    string ts = (full_process ? utils::full_process(s) : s);

    auto tokens = utils::split_string(s);

    std::sort(tokens.begin(), tokens.end());
    string sorted = "";
    for (auto token : tokens)
        sorted += token + (token != tokens.back() ? " " : "");

    return utils::strip(sorted);
}

int token_sort_ratio(const string &s1, const string &s2, bool full_proccess = true)
{
    /* NOTE: do we need force_ascii? */
    string sorted1 = proccess_and_sort(s1, full_proccess);
    string sorted2 = proccess_and_sort(s2, full_proccess);

    return ratio(sorted1, sorted2);
}

int token_sort_partial_ratio(const string &s1, const string &s2, bool full_proccess = true)
{
    /* NOTE: do we need force_ascii? */
    string sorted1 = proccess_and_sort(s1, full_proccess);
    string sorted2 = proccess_and_sort(s2, full_proccess);

    return partial_ratio(sorted1, sorted2);
}

}  // ns fuzz
