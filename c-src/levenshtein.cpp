/*
 * This file has been altered to better fit fuzzywuzzy.
 * To se all changes done, please diff this file with
 * <https://github.com/Tmplt/python-Levenshtein/blob/master/Levenshtein.c>
 *
 * Summary:
 *   - stripped all python-related code and data types;
 *   - fixed some spelling errors.
 */

/*
 * Levenshtein.c
 * @(#) $Id: Levenshtein.c,v 1.41 2005/01/13 20:05:36 yeti Exp $
 * Python extension computing Levenshtein distances, string similarities,
 * median strings and other goodies.
 *
 * Copyright (C) 2002-2003 David Necas (Yeti) <yeti@physics.muni.cz>.
 *
 * The Taus113 random generator:
 * Copyright (C) 2002 Atakan Gurkan
 * Copyright (C) 1996, 1997, 1998, 1999, 2000 James Theiler, Brian Gough
 * (see below for more)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 **/

/**
 * TODO:
 *
 * - Implement weighted string averaging, see:
 *   H. Bunke et. al.: On the Weighted Mean of a Pair of Strings,
 *         Pattern Analysis and Applications 2002, 5(1): 23-30.
 *   X. Jiang et. al.: Dynamic Computations of Generalized Median Strings,
 *         Pattern Analysis and Applications 2002, ???.
 *   The latter also contains an interesting median-search algorithm.
 *
 * - Deal with stray symbols in greedy median() and median_improve().
 *   There are two possibilities:
 *    (i) Remember which strings contain which symbols.  This allows certain
 *        small optimizations when processing them.
 *   (ii) Use some overall heuristics to find symbols which don't worth
 *        trying.  This is very appealing, but hard to do properly
 *        (requires some inequality strong enough to allow practical exclusion
 *        of certain symbols -- at certain positions)
 *
 * - Editops should be an object that only *looks* like a list (which means
 *   it is a list in duck typing) to avoid never-ending conversions from
 *   Python lists to LevEditOp arrays and back
 *
 * - Optimize munkers_blackman(), it's pretty dumb (no memory of visited
 *   columns/rows)
 *
 * - Make it really usable as a C library (needs some wrappers, headers, ...,
 *   and maybe even documentation ;-)
 *
 * - Add interface to various interesting auxiliary results, namely
 *   set and sequence distance (only ratio is exported), the map from
 *   munkers_blackman() itself, ...
 *
 * - Generalizations:
 *   - character weight matrix/function
 *   - arbitrary edit operation costs, decomposable edit operations
 *
 * - Create a test suite
 *
 * - Add more interesting algorithms ;-)
 *
 * Postponed TODO (investigated, and a big `but' was found):
 *
 * - A linear approximate set median algorithm:
 *   P. Indyk: Sublinear time algorithms for metric space problems,
 *         STOC 1999, http://citeseer.nj.nec.com/indyk00sublinear.html.
 *   BUT: The algorithm seems to be advantageous only in the case of very
 *   large sets -- if my estimates are correct (the article itself is quite
 *   `asymptotic'), say 10^5 at least.  On smaller sets either one would get
 *   only an extermely rough median estimate, or the number of distance
 *   computations would be in fact higher than in the dumb O(n^2) algorithm.
 *
 * - Improve setmedian() speed with triangular inequality, see:
 *   Juan, A., E. Vidal: An Algorithm for Fast Median Search,
 *         1997, http://citeseer.nj.nec.com/article/juan97algorithm.html
 *   BUT: It doesn't seem to help much in spaces of high dimension (see the
 *   discussion and graphs in the article itself), a few percents at most,
 *   and strings behave like a space with a very high dimension (locally), so
 *   who knows, it probably wouldn't help much.
 *
 **/

#ifndef _GNU_SOURCE
#  define _GNU_SOURCE
#endif

#include <string.h>
#include <math.h>
/* for debugging */
#include <stdio.h>

#include <assert.h>
#include "levenshtein.hpp"

namespace logarithm {

/**
 * lev_edit_distance:
 * @len1: The length of @string1.
 * @string1: A sequence of bytes of length @len1, may contain NUL characters.
 * @len2: The length of @string2.
 * @string2: A sequence of bytes of length @len2, may contain NUL characters.
 * @xcost: If nonzero, the replace operation has weight 2, otherwise all
 *         edit operations have equal weights of 1.
 *
 * Computes Levenshtein edit distance of two strings.
 *
 * Returns: The edit distance.
 **/
size_t
lev_edit_distance(size_t len1, const lev_byte *string1,
                  size_t len2, const lev_byte *string2,
                  int xcost) {
    size_t i;
    size_t *row;  /* we only need to keep one row of costs */
    size_t *end;
    size_t half;

    /* strip common prefix */
    while (len1 > 0 && len2 > 0 && *string1 == *string2) {
        len1--;
        len2--;
        string1++;
        string2++;
    }

    /* strip common suffix */
    while (len1 > 0 && len2 > 0 && string1[len1 - 1] == string2[len2 - 1]) {
        len1--;
        len2--;
    }

    /* catch trivial cases */
    if (len1 == 0)
        return len2;
    if (len2 == 0)
        return len1;

    /* make the inner cycle (i.e. string2) the longer one */
    if (len1 > len2) {
        size_t nx = len1;
        const lev_byte *sx = string1;
        len1 = len2;
        len2 = nx;
        string1 = string2;
        string2 = sx;
    }
    /* check len1 == 1 separately */
    if (len1 == 1) {
        if (xcost)
            return len2 + 1 - 2 * (memchr(string2, *string1, len2) != NULL);
        else
            return len2 - (memchr(string2, *string1, len2) != NULL);
    }
    len1++;
    len2++;
    half = len1 >> 1;

    /* initialize first row */
    row = (size_t *) malloc(len2 * sizeof(size_t));
    if (!row)
        return (size_t) (-1);
    end = row + len2 - 1;
    for (i = 0; i < len2 - (xcost ? 0 : half); i++)
        row[i] = i;

    /* go through the matrix and compute the costs.  yes, this is an extremely
     * obfuscated version, but also extremely memory-conservative and relatively
     * fast.  */
    if (xcost) {
        for (i = 1; i < len1; i++) {
            size_t *p = row + 1;
            const lev_byte char1 = string1[i - 1];
            const lev_byte *char2p = string2;
            size_t D = i;
            size_t x = i;
            while (p <= end) {
                if (char1 == *(char2p++))
                    x = --D;
                else
                    x++;
                D = *p;
                D++;
                if (x > D)
                    x = D;
                *(p++) = x;
            }
        }
    } else {
        /* in this case we don't have to scan two corner triangles (of size len1/2)
         * in the matrix because no best path can go thought them. note this
         * breaks when len1 == len2 == 2 so the memchr() special case above is
         * necessary */
        row[0] = len1 - half - 1;
        for (i = 1; i < len1; i++) {
            size_t *p;
            const lev_byte char1 = string1[i - 1];
            const lev_byte *char2p;
            size_t D, x;
            /* skip the upper triangle */
            if (i >= len1 - half) {
                size_t offset = i - (len1 - half);
                size_t c3;

                char2p = string2 + offset;
                p = row + offset;
                c3 = *(p++) + (char1 != *(char2p++));
                x = *p;
                x++;
                D = x;
                if (x > c3)
                    x = c3;
                *(p++) = x;
            } else {
                p = row + 1;
                char2p = string2;
                D = x = i;
            }
            /* skip the lower triangle */
            if (i <= half + 1)
                end = row + len2 + i - half - 2;
            /* main */
            while (p <= end) {
                size_t c3 = --D + (char1 != *(char2p++));
                x++;
                if (x > c3)
                    x = c3;
                D = *p;
                D++;
                if (x > D)
                    x = D;
                *(p++) = x;
            }
            /* lower triangle sentinel */
            if (i <= half) {
                size_t c3 = --D + (char1 != *char2p);
                x++;
                if (x > c3)
                    x = c3;
                *p = x;
            }
        }
    }

    i = *end;
    free(row);
    return i;
}

}  // ns logarithm
