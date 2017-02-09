#include "algorithm.hpp"
#include "utils.hpp"

#include <vector>
#include <tuple>

using std::vector;

size_t algorithm::levenshtein_distance(const string_view &s1, const string_view &s2)
{
    size_t len1 = s1.size();
    size_t len2 = s2.size();

    size_t i, j;

    /* Base case: a string is empty. */
    if (len1 == 0) return len2;
    if (len2 == 0) return len1;

    /*
     * Initialize rows[i].first (the previous row of distances).
     * This first row represents the edit distance againts an
     * empty string, so the distance is just the numbers of
     * characters to delete from the non-empty string.
     */
    vector<std::pair<size_t, size_t>> rows(len2 + 1);
    for (i = 0; i < rows.size(); i++)
        rows[i].first = i;

    for (i = 0; i < len1; i++) {
        /* Edit distance is delete (i+1) chars from non-empty to empty. */
        rows[0].second = i + 1; // WHY: what does this line do?

        /* Fill in the rest of the row. */
        for (j = 0; j < len2; j++) {
            auto cost = (s1[i] == s2[j]) ? 0 : 1;

            rows[j + 1].second = utils::min(rows[j].second + 1,
                                            rows[j + 1].first + 1,
                                            rows[j].first + cost);
        }

        /*
         * Copy the current row to the previous
         * one in preparation for next iteration.
         */
        for (auto &row : rows)
            row.first = row.second;
    }

    return rows[len2].second;
}
