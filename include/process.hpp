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
 * Finds the best matches in a vector of choses. Returns a vector of pairs where
 * containing the match and the score.
 */
vector<pair<string, int>> extractWithoutOrder(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, bool)> scorer=weighted_ratio
    , int score_cutoff=0);

/*
 * Convenience function for getting the choices with best scores.
 */
vector<pair<string, int>> extractBests(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, bool)> scorer=weighted_ratio
    , int score_cutoff = 0, intmax_t limit = 5);

/*
 * Convenience function for getting the choices with best scores.
 */
vector<pair<string, int>> extract(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, bool)> scorer=weighted_ratio
    , intmax_t limit = 5);

/*
 * This is a convenience method which returns the single best choice.
 *  See extractWithoutOrder() for the full arguments list.
 */
vector<pair<string, int>> extractOne(const string& query, const vector<string>& choices
    , function<string(string)> processor=utils::full_process, function<int(string, string, bool)> scorer=weighted_ratio
    , int score_cutoff = 0);

} // ns fuzz