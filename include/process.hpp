#pragma once

#include "common.hpp"
#include <utility>
#include <functional>

#include "utils.hpp"
#include "fuzzywuzzy.hpp"

namespace fuzz
{
using std::pair;
using std::function;
/*
 * Finds the best matches in a vector of choises. Returns a vector of pairs which
 * contains the matches and their respective scores.
 */
vector<pair<string, int>> extractWithoutOrder(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, const bool)> scorer=weighted_ratio
    , int score_cutoff=0);

/*
 * Convenience function for getting the choices with best scores.
 */
vector<pair<string, int>> extractBests(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, const bool)> scorer=weighted_ratio
    , int score_cutoff = 0, intmax_t limit = 5);

/*
 * Convenience function for getting the choices with best scores.
 */
vector<pair<string, int>> extract(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, const bool)> scorer=weighted_ratio
    , intmax_t limit = 5);

/*
 * This is a convenience method which returns the single best choice.
 */
vector<pair<string, int>> extractOne(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, const bool)> scorer=weighted_ratio
    , int score_cutoff = 0);
/*
 * This convenience function takes a list of strings containing duplicates and uses fuzzy matching to identify
 * and remove duplicates. Specifically, it uses the process.extract to identify duplicates that
 * score greater than a user defined threshold. Then, it looks for the longest item in the duplicate list
 * since we assume this item contains the most entity information and returns that. It breaks string
 * length ties on an alphabetical sort.
 * 
 * Note: as the threshold DECREASES the number of duplicates that are found INCREASES. This means that the
 *     returned deduplicated list will likely be shorter. Raise the threshold for fuzzy_dedupe to be less
 *     sensitive. 
 */
vector<string> dedupe(const vector<string>& contains_dupes, int threshold=70
    , function<int(string, string, const bool)> scorer=token_set_ratio);

} // ns fuzz

