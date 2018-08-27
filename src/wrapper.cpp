#include <iostream>

#include "wrapper.hpp"

namespace wrapper {

double ratio(const string &str1, const string &str2)
{
    size_t len1 = str1.length(),
           len2 = str2.length();

    const lev_byte *lb1 = reinterpret_cast<const lev_byte *>(str1.c_str()),
                   *lb2 = reinterpret_cast<const lev_byte *>(str2.c_str());

    size_t lensum = len1 + len2;
    size_t edit_dist = lev_edit_distance(len1, lb1, len2, lb2, 1);

    return static_cast<double>(lensum - edit_dist) / static_cast<double>(lensum);
}

vector<LevOpCode> get_opcodes(string &s1, string &s2)
{
    vector<LevOpCode> opcodes;
    size_t len1, len2, nb, n;
    const lev_byte *lb1, *lb2;
    LevEditOp *ops;
    LevOpCode *bops;

    len1 = s1.length();
    len2 = s2.length();

    lb1 = reinterpret_cast<const lev_byte *>(s1.c_str());
    lb2 = reinterpret_cast<const lev_byte *>(s2.c_str());

    ops = lev_editops_find(len1, lb1, len2, lb2, &n);
    if (ops != nullptr) {
        bops = lev_editops_to_opcodes(n, ops, &nb, len1, len2);
        if (bops != nullptr) {
            opcodes.assign(bops, bops + nb);
            free(bops);
        }
        free(ops);
    }

    return opcodes;
}

vector<LevEditOp> get_editops(string &s1, string &s2)
{
    vector<LevEditOp> editops;
    size_t len1, len2, n;
    const lev_byte *lb1, *lb2;
    LevEditOp *ops;

    len1 = s1.length();
    len2 = s2.length();

    lb1 = reinterpret_cast<const lev_byte *>(s1.c_str());
    lb2 = reinterpret_cast<const lev_byte *>(s2.c_str());

    ops = lev_editops_find(len1, lb1, len2, lb2, &n);
    if (ops != nullptr) {
        editops.assign(ops, ops + n);
        free(ops);
    }

    return editops;
}

vector<LevOpCode> get_opcodes(vector<LevEditOp> &v, string &s1, string &s2)
{
    vector<LevOpCode> opcodes;
    size_t len1, len2, n;
    LevEditOp *ops;
    LevOpCode *bops;

    n = v.size();
    len1 = s1.length();
    len2 = s2.length();

    ops = v.data();
    bops = lev_editops_to_opcodes(n, ops, &n, len1, len2);
    if (bops != nullptr) {
        opcodes.assign(bops, bops + n);
        free(bops);
    }

    return opcodes;
}

vector<LevMatchingBlock> get_matching_blocks(vector<LevOpCode> &v, string &s1, string &s2)
{
    vector<LevMatchingBlock> blocks;
    size_t n, nmb, len1, len2;
    LevMatchingBlock *mblocks;

    n = v.size();
    len1 = s1.length();
    len2 = s2.length();

    mblocks = lev_opcodes_matching_blocks(len1, len2, n, v.data(), &nmb);
    if (mblocks != nullptr) {
        blocks.assign(mblocks, mblocks + nmb);
        free(mblocks);
    }

    return blocks;
}

}  // ns wrapper
