#include "algorithm.hpp"

#include <vector>
#include <tuple>

using std::vector;

namespace algorithm {

size_t algorithm::lev_dist(const string_view &s1, const string_view &s2)
{
    size_t lena = s1.size();
    size_t lenb = s2.size();

    /* Base case: a string is empty. */
    if (lena == 0) return lenb;
    if (lenb == 0) return lena;

    /*
     * Initialize rows[i].first (the previous row of distances).
     * This first row represents the edit distance againts an
     * empty string, so the distance is just the numbers of
     * characters to delete from the non-empty string.
     */
    vector<std::pair<size_t, size_t>> rows(lenb + 1);
    for (size_t i = 0; i < rows.size(); i++)
        rows[i].first = i;

    for (size_t i = 0; i < lena; i++) {
        /* Edit distance is delete (i+1) chars from non-empty to empty. */
        rows[0].second = i + 1;

        /* Fill in the rest of the row. */
        for (size_t j = 0; j < lenb; j++) {
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

    return rows[lenb].second;
}

}
