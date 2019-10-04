#pragma once

#include "common.hpp"

namespace /* I'm in your mind... */ fuzz {

/*                          */
/* Basic scoring functions. */
/*                          */

/* Calculates a Levenshtein simple ratio between the string. */
unsigned int ratio(const string &s1, const string &s2, const bool full_process = true);

/*
 * Return the ratio of the most similar substring
 * as a number between 0 and 100.
 */
unsigned int partial_ratio(const string &s1, const string &s2, const bool full_process = true);

/*                             */
/* Advanced scoring functions. */
/*                             */

/*
 * Returns a measure of the strings' similarity between 0 and 100
 * but sorting the token before comparing.
 */
unsigned int token_sort_ratio(const string &s1, const string &s2, const bool full_process = true);
unsigned int token_sort_partial_ratio(const string &s1, const string &s2, const bool full_process = true);

/*
 * Splits the strings into tokens and computes intersections and
 * remainders between the tokens of the two strings. A comparison string
 * is then built up and is compared using the simple ratio algorithm.
 * Useful for strings where words appear redundantly.
 */
unsigned int token_set_ratio(const string &s1, const string &s2, const bool full_process = true);

/*
 * Returns the ratio of the most similar substring as a number
 * between 0 and 100 but sorting the token before comparing.
 */
unsigned int partial_token_set_ratio(const string &s1, const string &s2, const bool full_process = true);

/*                 */
/* Combination API */
/*                 */

/*
 * Quick ratio comparison between two strings.
 * Runs utils::full_process on both strings.
 * Short circuits if either string is empty after processing.
 */
unsigned int quick_ratio(const string &s1, const string &s2, const bool full_process = true);

/*
 * Returns a measure of the strings' similarity between 0 and 100, using different algorithms.
 *
 * Steps in the order they occur:
 *  #. Run utils::full_process on both strings
 *  #. Short circuit if either string is empty
 *  #. Take the ratio of the two processed strings
 *  #. Run checks to compare the length of the strings:
 *    * If one of the strings is more than 1.5 times as long as the other,
 *      use partial_ratio comparisons -- scale partial results by 0.9
 *      (this makes sure only full results can return 100)
 *    * If one of the strings is over 8 times as long as the other,
 *      scale by 0.6 instead
 *
 *  #. Run the other ratio functions
 *    * If using partial ratio functions, call partial_ratio,
 *      partial_token_sort_ratio and partial_token_set_ratio.
 *      Then scale all of these by the ratio based on length.
 *    * Otherwise call token_sort_ratio and token_set_ratio
 *      and scale these results by 0.95 (on top of any partial scalars)
 *
 *  #. Take the highest value from these results, round it, and return
 *     as an integer.
 */
unsigned int weighted_ratio(const string &s1, const string &s2, const bool full_process = true);

/* I'm not in your mind */ }
