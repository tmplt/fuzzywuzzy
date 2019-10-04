#include "process.hpp"

#include <set>

namespace fuzz
{

using std::set;

vector<pair<string, int>> extractWithoutOrder(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, const bool)> scorer
    , int score_cutoff)
{
    string processed_query = processor(query);

    /* TODO: Avoid running full_process twice. */
    
    auto score_func = [&scorer] (const string& s1, const string& s2) { return scorer(s1, s2, false); };
    auto pre_processor = utils::full_process;

    /* NOTE: Why? But the Python version does the following. */
    /* processed_query = pre_processor(processed_query) */

    vector<pair<string, int>> results;
    for(const auto& choice : choices) {
        string processed = pre_processor(processor(choice));
        int score = score_func(processed_query, processed);
        if(score >= score_cutoff)
            results.emplace_back(choice, score);
    }

    return results;
}

vector<pair<string, int>> extractBests(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, const bool)> scorer
    , int score_cutoff, intmax_t limit)
{
    auto sl = extractWithoutOrder(query, choices, processor, scorer, score_cutoff);
    if(limit == -1)
        return sl;

    std::partial_sort(sl.begin(), sl.begin()+limit, sl.end(), [](const auto& a, const auto& b){ return a.second > b.second; });

    /* If limit < 0, it means to return everything. Since vector::size() is always */
    /* larger than -1, we can combine the check. */
    if(sl.size() > limit) {
        sl.resize((size_t)limit);
        sl.shrink_to_fit();
    }
    return sl;
}

vector<pair<string, int>> extract(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, const bool)> scorer
    , intmax_t limit)
{
    return extractBests(query, choices, processor, scorer, 0, limit);
}

vector<pair<string, int>> extractOne(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, const bool)> scorer
    , int score_cutoff)
{
    return extractBests(query, choices, processor, scorer, score_cutoff, 1);
}

vector<string> dedupe(const vector<string>& contains_dupes, int threshold, function<int(string, string, const bool)> scorer)
{
    /* NOTE: This function is a translation of the python and it can be optimized a lot. The original algorithm is */
    /* far from ideal. */
    vector<string> extractor;

    for(const auto& str : contains_dupes) {
        auto matches = extract(str, contains_dupes, utils::full_process, scorer, -1);

        vector<string> filtered;
        for(size_t i=0;i<matches.size();i++) {
            int score = matches[i].second;
            string value = matches[i].first;
            if(score > threshold)
                filtered.push_back(value);
        }

        /* if there is only 1 item in *filtered*, no duplicates were found so append to *extracted* */
        if(filtered.size() == 1)
            extractor.push_back(*filtered.begin());
        else {
            /* alpha sort */
            std::stable_sort(filtered.begin(), filtered.end(), [](const auto& a, const auto& b){ return a[0] > b[0]; });

            /* length sort */
            std::stable_sort(filtered.begin(), filtered.end(), [](const auto& a, const auto& b){ return a.size() > b.size(); });

            /* take first item as our 'canonical example' */
            extractor.push_back(*filtered.begin());
        }
    }

    // uniquify *extractor* list
    set<string> keys;
    for(auto str : extractor)
        keys.insert(str);
        
    /* check that extractor differs from contain_dupes (e.g. duplicates were found) */
    /* if not, then return the original list */
    if(keys.size() == contains_dupes.size())
        return contains_dupes;
    else
        return vector<string>(keys.begin(), keys.end());  
}

}  // ns fuzz

