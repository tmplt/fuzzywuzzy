#include <iostream>

#include "diffutils.hpp"

namespace diffutils {

double ratio(const string &str1, const string &str2)
{
    size_t len1 = str1.length(),
           len2 = str2.length();

    const lev_byte *lb1 = reinterpret_cast<const lev_byte *>(str1.c_str()),
                   *lb2 = reinterpret_cast<const lev_byte *>(str2.c_str());

    size_t lensum = len1 + len2;
    size_t edit_dist = lev_edit_distance(len1, lb1, len2, lb2, 1);

    return static_cast<double>(lensum - edit_dist) / lensum;
}

vector<LevOpCode> get_opcodes(string &s1, string &s2)
{
    size_t len1, len2, nb, n;
    const lev_byte *lb1, *lb2;
    LevEditOp *ops;
    LevOpCode *bops;

    len1 = s1.length();
    len2 = s2.length();

    lb1 = reinterpret_cast<const lev_byte *>(s1.c_str());
    lb2 = reinterpret_cast<const lev_byte *>(s2.c_str());

    ops = lev_editops_find(len1, lb1, len2, lb2, &n);
    bops = lev_editops_to_opcodes(n, ops, &nb, len1, len2);
    free(ops);

    /* LEAK: do we have to free(bops) here, or does the returned vector own it now? */
    return {bops, bops + nb};
}

vector<LevOpCode> get_opcodes(vector<LevEditOp> &v, string &s1, string &s2)
{
    size_t len1, len2, n;
    LevEditOp *ops;
    LevOpCode *bops;

    n = v.size();
    len1 = s1.length();
    len2 = s2.length();

    ops = v.data();
    bops = lev_editops_to_opcodes(n, ops, &n, len1, len2);
    free(ops);

    /* LEAK: same here? */
    return {bops, bops + n};
}

vector<LevMatchingBlock> get_matching_blocks(vector<LevOpCode> &v, string &s1, string &s2)
{
    size_t n, nmb, len1, len2;
    LevMatchingBlock *mblocks;

    n = v.size();
    len1 = s1.length();
    len2 = s2.length();

    mblocks = lev_opcodes_matching_blocks(len1, len2, n, v.data(), &nmb);

    /* LEAK: same here? */;
    return {mblocks, mblocks + nmb};
}

}  // ns diffutils
