#include "diffutils.hpp"
#include "algorithm.hpp"
#include "structs.hpp"
#include "common.hpp"

namespace diffutils {

double ratio(const string_view &s1, const string_view &s2)
{
    size_t len1 = s1.length(),
           len2 = s2.length();

    size_t lensum = len1 + len2;
    size_t edit_dist = algorithm::levenshtein_distance(s1, s2);

    return (lensum - edit_dist) / static_cast<double>(lensum);
}

vector<matching_block> matching_blocks(const vector<op_code> ops, const size_t len1,
        __attribute__((unused)) const size_t len2)
{
    /* number of matching blocks. */
    size_t nmb = 0;

    /* Compute the number of matching blocks. */
    for (auto o = ops.cbegin(); o != ops.cend(); o++) {
        if (o->type == keep) {
            nmb++;

            /*
             * Adjacent keep blocks -- apparently
             * never produced, but...
             */
            while (o->type == keep) o++;
            if (o != ops.cend()) break;
        }
    }

    /* Do the convertion. */
    vector<matching_block> mblocks(nmb + 1); // WHY: is the +1 neccesary?
    auto mb = mblocks.begin();

    for (auto o = ops.cbegin(); o != ops.cend(); o++) {
        if (o->type == keep) {
            mb->spos = o->sbeg;
            mb->dpos = o->dbeg;

            /* Adjacent keep blocks -- same song here. */
            while (o->type == keep) o++;

            if (o != ops.cend()) {
                mb->length = len1 - mb->spos;
                mb++;
                break;
            }

            mb->length = o->sbeg - mb->spos;
            mb++;
        }
    }

    return mblocks;
}

}  // ns diffutils
