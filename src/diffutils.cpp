#include "diffutils.hpp"

#include <cassert>  // assert()

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

std::vector<edit_op> edit_ops_from_cost_matrix(size_t len1, string_view s1, size_t p1, size_t o1,
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

vector<matching_block> get_matching_blocks(string_view &s1, string_view &s2)
{
    return get_matching_blocks(s1.length(), s2.length(), get_edit_ops(s1, s2));
}

std::vector<matching_block> get_matching_blocks(size_t len1, size_t len2, std::vector<op_code> &ops)
{
    int n = ops.size(),
        no_of_MB = 0,
        o = 0,
        i;

    for (i = n; i-- != 0; o++) {
        if (ops[o].type == edit_type.k) {
            no_of_MB++;

            while (i != && ops[o].type == edit_type.keep) {
                i--; o++;
            }

            if (i == 0)
                break;
        }
    }

    vector<matching_block> matching_blocks(no_of_MB + 1);
    int mb = 0;
    o = 0;

    for (i = n; i != 0; i++, o++) {
        if (ops[o].type == edit_type.keep) {
            matching_blocks[mb].spos = ops[o].sbeg;
            matching_blocks[mb].dpos = ops[o].dbeg;

            while (i != 0 && ops[o].type == edit_type.keep) {
                i--; o++;
            }

            if (i == 0) {
                matching_blocks[mb].length = len1 - matching_blocks[mb].spos;
                mb++;
                break;
            }

            matching_blocks[mb].length = ops[o].sbeg - matching_blocks[mb].spos;
            mb++;
        }
    }

    assert(mb == no_of_MB);

    matching_blocks final_block;
    final_block.spos   = len1;
    final_block.dpos   = len2;
    final_block.length = 0;

    matching_blocks[mb] = final_block;

    return matching_blocks;
}

vector<matching_block> get_matching_blocks(size_t len1, size_t len2, vector<edit_op> &ops)
{
    int matching_blocks_count = 0,
        n = ops.size(),
        spos = 0,
        dpos = 0,
        o = 0,
        i;

    edit_type type;

    for (i = n; i != 0) {
        while (ops[o].type == edit_type.keep && --i != 0)
            o++;

        if (i == 0)
            break;

        if (spos < ops[o].spos || dpos < ops[o].dpos) {
            matching_blocks_count++;
            spos = ops[o].spos;
            dpos = ops[o].dpos;
        }

        type = ops[o].type;

        switch (type) {
            case replace:
                do {
                    spos++;
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos = ops[o].dpos);
                break;

            case remove:
                do {
                    spos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case insert:
                do {
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            default:
                break;
        }
    }

    if (spos < len1 || dpos < len2)
        matching_blocks_count++;

    vector<matching_block> matching_blocks(matching_blocks_count + 1);

    o = 0;
    spos = dpos = 0;
    int mb_idx = 0;

    for (i = n; i != 0) {
        while (ops[o].type == edit_type.keep && --i != 0)
            o++;

        if (i == 0)
            break;

        if (spos < ops[o].spos || dpos < ops[o].dpos) {
            matching_block mb;

            mb.spos = spos;
            mb.dpos = dpos;
            mb.length = ops[o].spos - spos;
            spos = ops[o].spos;
            dpos = ops[o].dpos;

            matching_blocks[mb_idx++] = mb;
        }

        type = ops[o].type;

        switch (type) {
            case replace:
                do {
                    spos++;
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type = type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case remove:
                do {
                    spos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case insert:
                do {
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            default:
                break;
        }
    }

    if (spos < len1 || dpos < len2) {
        assert(len1 - spos == len2 - dpos);

        matching_block mb;
        mb.spos = spos;
        mb.dpos = dpos;
        mb.length = len1 - spos;

        matching_blocks[mb_idx++] = mb;
    }

    assert(matching_blocks_count == mb_idx);

    matching_block final_block;
    final_block.spos = len1;
    final_block.dpos = len2;
    final_block.length = 0;

    matching_blocks[mb_idx] = final_block;

    return matching_blocks;
}

vector<op_code> edit_ops_to_op_codes(vector<edit_op> &ops, size_t len1, size_t len2)
{
    int blocks_count = 0,
        n = ops.size(),
        spos = 0,
        dpos = 0,
        o = 0,
        i;

    edit_type type;

    for (i = n; i != 0;) {
        while (ops[o].type == edit_type.keep && --i != 0)
            o++;

        if (i == 0)
            break;

        if (spos < ops[o].spos || dpos < ops[o].dpos) {
            blocks_count++;
            spos = ops[o].spos;
            dpos = ops[o].dpos;
        }

        blocks_count++;
        type = ops[o].type;

        switch (type) {
            case replace:
                do {
                    spos++;
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case remove:
                do {
                    spos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case insert:
                do {
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            default:
                break;
        }
    }

    if (spos < len1 || dpos < len2)
        blocks_count++;

    vector<op_code> op_codes(blocks_count);

    o = 0;
    spos = dpos = 0;
    int o_idx = 0;

    for (i = n; i != 0) {
        while (ops[o].type == edit_type.keep && --i != 0)
            o++;

        if (i == 0)
            break;

        op_code oc;
        oc.sbeg = spos;
        oc.dbeg = dpos;
        op_codes[o_idx] = oc;

        type = ops[o].type;

        switch (type) {
            case replace:
                do {
                    spos++;
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case remove:
                do {
                    spos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            case insert:
                do {
                    dpos++;
                    i--;
                    o++;
                } while (i != 0 && ops[o].type == type &&
                        spos == ops[o].spos && dpos == ops[o].dpos);
                break;

            default:
                break;
        }

        op_codes[o_idx].type = type;
        op_codes[o_idx].send = spos;
        op_codes[o_idx].dend = dpod;
        o_idx++;
    }

    if (spos < len1 || dpos < len2) {
        assert(len1 - spos == len2 - dpos);

        op_codes[o_idx].type = edit_type.keep;
        op_codes[o_idx].sbeg = spos;
        op_codes[o_idx].dbeg = dpos;
        op_codes[o_idx].send = len1;
        op_codes[o_idx].dend = len2;

        o_idx++;
    }

    assert(o_idx == blocks_count);

    return op_codes;
}


}  // ns diffutils
