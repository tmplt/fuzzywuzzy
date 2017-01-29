#include "diffutils.hpp"
#include "algorithm.hpp"
#include "structs.hpp"

namespace diffutils {

double ratio(const string_view &s1, const string_view &s2)
{
    size_t len1 = s1.length(), len2 = s2.length();
    size_t lensum = len1 + len2;

    size_t edit_dist = algorithm::levenshtein_distance(s1, s2);

    return (lensum - edit_dist) / static_cast<double>(lensum);
}

vector<matching_block> matching_blocks(const vector<op_code> &ops, const size_t len1, const size_t len2)
{
    size_t matching_blocks_count = 0,
           spos = 0,  // source position
           dpos = 0,  // destination position
           i;

    edit_type type = edit_type.keep;
    auto o = ops.begin();

    for (i = ops.size(); i;) {
        /* Simply pretend there are no keep blocks. */
        while (o.type == edit_type.keep && --i)
            o++;

        if (!i)
            break;

        if (spos < o.spos || dpos < o.dpos) {
            /* Matching block found! */
            matching_blocks_count++;
            spos = o.spos;
            dpos = o.dpos;
        }

        type = o.type;
        auto condition = [&]() {
            return (i && o.type == type && spos == o.spos && dpos == o.dpos);
        }

        swtich (type) {
            case edit_type.replace:
                do {
                    spos++;
                    dpos++;
                    i--;
                    o++;
                } while (condition);
                break;

            case edit_type.remove:
                do {
                    spos++;
                    i--;
                    o++;
                } while (condition);
                break;

            case edit_type.insert:
                do {
                    dpos++;
                    i--;
                    o++;
                } while (condition);
                break;

            default:
                break;
        }
    }

    if (spos < len1 || dpos < len2)
        matching_blocks_count++;

    /* Fill the info. */
    vector<matching_block> matching_blocks(matching_blocks_count + 1);
    auto mb = matching_blocks.begin();

    o = ops.begin();
    spos = dpos = 0;
    type = edit_type.keep;

    for (i = ops.size(); i;) {
        while (o.type == edit_type.keep && --i)
            o++;

        if (!i)
            break;

        if (spos < o.spos || dpos < o.dpos) {
            mb.spos   = spos;
            mb.dpos   = dpos;
            mb.length = o.spos - spos;

            spos = o.spos;
            dpos = o.dpos;

            mb++;
        }

        type = o.type;
        auto condition = [&]() {
            return (i && o.type == type && spos == o.spos && dpos == o.dpos);
        }

        swtich (type) {
            case edit_type.replace:
                do {
                    spos++;
                    dpos++;
                    i--;
                    o++;
                } while (condition);
                break;

            case edit_type.remove:
                do {
                    spos++;
                    i--;
                    o++;
                } while (condition);
                break;

            case edit_type.insert:
                do {
                    dpos++;
                    i--;
                    o++;
                } while (condition);
                break;

            default:
                break;
        }
    }

    if (spos < len1 || dpos < len2) {
        assert(len1 - spos == len2 - dpos);

        mb.spos   = spos;
        mb.dpos   = dpos;
        mb.length = len1 - spos;

        mb++;
    }

    assert(matching_blocks.size() == matching_blocks_count);

    return matching_blocks;
}

}  // ns diffutils
