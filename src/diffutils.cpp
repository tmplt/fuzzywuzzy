#include "diffutils.hpp"

namespace diffutils {

std::vector<edit_op> get_edit_ops(string_view s1, string_view s2)
{
    size_t len1 = s1.size(), len2 = s2.size();

    /* Our "pointers". */
    /* auto p1 = s1.begin(), p2 = s2.begin(); */

    /* Our "pointers". */
    size_t p1 = 0, p2 = 0;

    size_t i;  // for our for-loops

    size_t len1o, len2o; // count of operations on respective string.

    len1o = 0;

    /* Strip common prefix. */
    while (len1 > 0 && len2 > 0 && s1[p1] == s2[p2]) {
        len1--;
        len2--;

        p1++;
        p2++;

        len1o++;
    }

    len2o = len1o;

    /* Strip common suffix. */
    while (len1 > 0 && len2 > 0 && s1[p1 + len1 - 1] == s2[p2 + len2 - 1]) {
        len1--;
        len2--;
    }

    len1++;
    len2++;

    vector<size_t> matrix(len2 * len1);

    for (i = 0; i < len2; i++)
        matrix[i] = i;
    for (i = 1; i < len1; i++)
        matrix[len2 * i] = i;

    for (i = 1; i < len1; i++) {
        size_t ptr_prev = (i - 1) * len2;
        size_t ptr_curr = i * len2;
        size_t ptr_end  = ptr_curr + len2 - 1;

        auto char1 = s1[p1 + i - 1];
        size_t ptr_char2 = p2;

        size_t x = i;

        ptr_curr++;

        while (ptr_curr <= ptr_end) {
            size_t c3 = matrix[ptr_prev++] + (char1 != s2[ptr_char2++] ? 1 : 0);
            x++;

            if (x > c3)
                x = c3;

            c3 = matrix[ptr_prev] + 1;

            if (x > c3)
                x = c3;

            matrix[ptr_curr++] = x;
        }
    }

    return edit_ops_from_cost_matrix(len1, s1, p1, len2, s2, p2, len2o, matrix);
}

std::vector<edit_op> edit_opts_from_cost_matrix(size_t len1, string_view s1, size_t p1, size_t o1,
                                                size_t len2, string_view s2, size_t p2, size_t o2,
                                                std::vector<size_t> &matrix)
{
    size_t i   = len1 - 1,
           j   = len2 - 1,
           pos = matrix[len1 * len2 - 1],
           ptr = len1 * len2 - 1;

    /*
     * A control variable?
     * Is only assigned as 0 or -1;
     */
    int dir = 0;

    std::vector<edit_op> ops;

    while (i > 0 || j > 0) {
        if (dir < 0 && j != 0 && matrix[ptr] == matrix[ptr - 1] + 1) {
            pos--;
            edit_op eop;

            eop.type = edit_type.insert;
            eop.spos = 1 + o1;
            eop.dpos = --j + o2;

            ops[pos] = eop;
            ptr--;

            continue;
        }

        if (dir > 0 && i != 0 && matrix[ptr] == matrix[ptr - len2] + 1) {
            pos--;
            edit_op eop;

            eop.type = edit_type.remove;
            eop.spos = --i + o1;
            eop.dpos = j + o2;

            ops[pos] = eop;
            ptr -= len2;

            continue;
        }

        if (i != 0 && j != 0 && matrix[ptr] == matrix[ptr - len2 - 1]
                && s1[p1 + i  - 1] == s2[p2 + j - 1] {
            i--;
            j--;
            ptr -= len2 + 1;
            dir = 0;

            continue;
        }

        if (i != 0 && j != 0 && matrix[ptr] == matrix[ptr - len2 - 1] + 1) {
            pos--;
            edit_op eop;

            eop.type = edit_type.replace;
            eop.spos = --i + o1;
            eop.dpos = --j + o2;

            ops[pos] = eop;
            ptr -= len2 + 1;
            dir = 0;

            continue;
        }

        if (dir == 0 && j != 0 && matrix[ptr] == matrix[ptr - 1] + 1) {
            pos--;
            edit_op eop;

            eop.type = edit_type.insert;
            eop.spos = 1 + o1;
            eop.dpos = --j + o2;

            ops[pos] = eop;
            ptr--;
            dir = -1;

            continue;
        }

        if (dir == 0 && i != 0 && matrix[ptr] == matrix[ptr - len2] + 1) {
            pos--;
            edit_op eop;

            eop.type = edit_type.remove;
            eop.spos = --i + o1;
            eop.dpos = j + o2;

            ops[pos] = eop;
            ptr -= len2;
            dir = 1;

            continue;
        }

        assert(false);  // oh
    }

    return ops;
}

std::vector<matching_block> get_matching_blocks(size_t len1, size_t len2)
{
    ...
}


}  // ns diffutils
