#pragma once

#include "common.hpp"

namespace /* I'm in your mind... */ fuzz {

/*                          */
/* Basic scoring functions. */
/*                          */

/* Calculates a Levenshtein simple ratio between the string. */
int ratio(const string_view &s1, const string_view &s2);

/*
 * Return the ratio of the most similar substring
 * as a number between 0 and 100.
 */
int partial_ratio(const string_view &s1, const string_view &s2);

/*                             */
/* Advanced scoring functions. */
/*                             */

/*
 * Returns a measure of the strings' similarity between 0 and 100
 * but sorting the token before comparing.
 */
int token_sort_ratio(string_view &s1, string_view &s2);
int token_sort_partial_ratio(string_view &s1, string_view &s2);

/*
 * Splits the strings into tokens and computes intersections and
 * remainders between the tokens of the two strings. A comparison string
 * is then built up and is compared using the simple ratio algorithm.
 * Useful for strings where words appear redundantly.
 */
int token_set_ratio(string_view &s1, string_view &s2);

/*
 * Returns the ratio of the most similar substring as a number
 * between 0 and 100 but sorting the token before comparing.
 */
int partial_token_sort_ratio(string_view &s1, string_view &s2);


/* Returns a measure of the strings' similarity between 0 and 100, using different algorithms. */
int weighted_ratio(string_view &s1, string_view &s2);

}
