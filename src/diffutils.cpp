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

    auto char1 = s1.cbegin(),
         char2 = s2.cbegin();

    /* Strip common prefix. */
    while (len1 > 0 && len2 > 0 && *char1 == *char2) {
        len1--;
        len2--;
        char1++;
        char2++;

        len1o++;
    }

    len2o = len1o;

    /* Strip common suffix. */
    for (auto strb1 = s1.back(), strb2 = s2.back(); *strb1 == *strb2;
            strb1++, strb2++) {
        len1--;
        len2--;
    }

    /* Initialize first row and column. */
    auto matrix = vector<size_t>(len1 * len2);
    auto m = matrix.begin();
    for (i = 0; i < len2; i++)
        matrix[i] = i;
    for (i = 1; i < len1; i++)
        matrix[len2 * i] = i;

    char1 = s1.cbegin();
    char2 = s2.cbegin();

    /* Find the costs and fill the matrix. */
    for (i = 1; i < len1; i++) {
        auto prev = m + (i - 1) * len2;
        auto p    = m + i * len2;
        auto end  = p + len2 - 1;

        const auto char1 = s1[i - 1];
        const auto char2p = s2.cbegin();

        size_t x = i;
        p++;
        while (p <= end) {
            size_t c3 = *(prev++) + (char1 != *(char2p++));
            x++;

            if (x > c3) x = c3;
            c3 = *prev + 1;
            if (x > x3) x = c3;
            *(p++) = x;
        }
    }

    /* Find the way back. */
    return editops(s1, len1o, s2, len2o, matrix);
}

/*
 * This is a verbatim copy of python-Levenshtein's editops_from_cost_matrix,
 * only altered to use vectors and string-like objects instead of raw c-strings.
 *
 */
vector<edit_op> editops(const string_view &s1, const size_t o1,
                        const string_view &s2, const size_t o2, const vector<size_t> &matrix)
{
    size_t pos = matrix[len1 * len2 - 1],
           i = len1 - 1,
           j = len2 - 1;

    int dir = 0;

    vector<edit_op> ops(matrix.size());
    auto p = matrix.cbegin() + (len1 * len2) - 1;

    while (i || j) {
        /* Prefer continuing in the same direction. */
        if (dir < 0 && j && *p == *(p - 1) + 1) {
            pos--;
            ops[pos].type = insert;
            ops[pos].spos = i + o1;
            ops[pos].dpos = --j + o2;
            continue;
        }

        if (dir > 0 && i && *p == *(p - len2) + 1) {
            pos--;
            ops[pos].type = erase;
            ops[pos].spos = --i + o1;
            ops[pos].dpos = j + o2;
            continue;
        }

        if (i && j && *p == *(p - len2 - 1) && s1[i - 1] == s2[j - 1]) {
            /* Don't be stupid like difflib, don't store edit_type.keep. */
            i--;
            j--;
            p -= len2 + 1;
            dir = 0;
            continue;
        }

        if (i && j && *p == *(p - len2 - 1) + 1) {
            pos--;
            ops[pos].type = replace;
            ops[pos].spos = --i + o1;
            ops[pos].dpos = --j + o2;
            p -= len2 + 1;
            dir = 0;
            continue;
        }

        /*
         * We can't turn directly from -1 to 1, in this case it would be better
         * to go diagonally, but check it (dir == 0).
         */
        if (dir == 0 && j && *p == *(p - 1) + 1) {
            pos--;
            ops[pos].type = insert;
            ops[pos].spos = i + o1;
            ops[pos].dpos = --j + o2;
            p--;
            dir = -1;
            continue;
        }

        if (dir == 0 && i && *p == *(p - len2) + 1) {
            pos--;
            ops[pos].type = erase;
            ops[pos].spos = --i + o1;
            ops[pos].dpos = j + o2;
            p -= len2;
            dir = 1;
            continue;
        }

        /* coredump right now, later might be too late. */
        assert(false && "lost in the cost matrix");
    }

    return ops;
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
