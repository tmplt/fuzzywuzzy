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

    return static_cast<double>(lensum - edit_dist) / lensum;
}

vector<edit_op> editops(const string_view &s1, const string_view &s2)
{
    size_t len1 = s1.length(),
           len2 = s2.length(),
           len1o = 0, /* operations on s1... */
           len2o, /* ... and on s2. */
           i;

    auto str1 = s1.cbegin(),
         str2 = s2.cbegin();

    /* Strip common prefix. */
    while (len1 > 0 && len2 > 0 && *str1 == *str2) {
        len1--;
        len2--;
        str1++;
        str2++;

        len1o++;
    }

    len2o = len1o;

    /* Strip common suffix. */
    for (auto strb1 = s1.back(), strb2 = s2.back(); *strb1 == *strb2;
            strb1++, strb2++) {
        len1--;
        len2--;
    }

    auto matrix = vector<size_t>(len1 * len2);
    auto m = matrix.begin();
    for (i = 0; i < len2; i++)
        matrix[i] = i;
    for (i = 1; i < len1; i++)
        matrix[len2 * i] = i;

    /* Find the costs and fill the matrix. */
    for (i = 1; i < len1; i++) {
        auto prev = m + (i - 1) * len2;
        auto p = m + i * len2;
        auto end = p + len2 - 1;


    }
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
            if (o == ops.cend()) break;
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

            if (o == ops.cend()) {
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

vector<op_code> opcodes(const string_view &s1, const string_view &s2)
{

}


}  // ns diffutils
