#include "process.hpp"

#include <set>

namespace fuzz
{

using std::set;

vector<pair<string, int>> extractWithoutOrder(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, bool)> scorer
    , int score_cutoff)
{
    // auto no_process = [](string x) { return x; };
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
            results.push_back(pair<string, int>{choice, score});
    }

    return results;
}

vector<pair<string, int>> extractBests(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, bool)> scorer
    , int score_cutoff, intmax_t limit)
{
    auto sl = extractWithoutOrder(query, choices, processor, scorer);
    if(limit == -1)
        return sl;

    std::partial_sort(sl.begin(), sl.begin()+limit, sl.end(), [](auto a, auto b){ return a.second > b.second; });
    return vector<pair<string, int>>(sl.begin(), sl.begin()+limit);
}

vector<pair<string, int>> extract(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, bool)> scorer
    , intmax_t limit)
{
    return extractBests(query, choices, processor, scorer, 0, limit);
}

vector<pair<string, int>> extractOne(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, bool)> scorer
    , int score_cutoff)
{
    return extractBests(query, choices, processor, scorer, score_cutoff, 1);
}

vector<string> dedupe(const vector<string>& contains_dupes, int threshold, function<int(string, string, bool)> scorer)
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
            extractor.push_back(str);
        else {
            /* alpha sort */
            std::sort(filtered.begin(), filtered.end(), [](auto a, auto b){ return a[0] > b[0]; });

            /* length sort */
            std::sort(filtered.begin(), filtered.end(), [](auto a, auto b){ return a.size() > b.size(); });

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

}  // ns huzz