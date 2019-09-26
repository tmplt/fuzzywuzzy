#include "process.hpp"

namespace fuzz
{

vector<pair<string, int>> extractWithoutOrder(const string& query, const vector<string>& choices
    , function<string(string)> processor, function<int(string, string, bool)> scorer
    , int score_cutoff)
{
    auto no_process = [](string x) { return x; };
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


}  // ns huzz