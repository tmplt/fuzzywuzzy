#include "utils.hpp"

#include <cmath>

namespace fuzz {

namespace utils {

/*
 * Return a rounded percentage in the range [0,100].
 */
unsigned int percent_round(double val)
{
    return intr(100 * val);
}

/*
 * Return a correctly rounded integer.
 */
unsigned int intr(double val)
{
    return static_cast<unsigned int>(std::round(val));
}

/*
 * Split a string into multiple strings when a character is met.
 * Returns all tokens in an array.
 */
vector<string> split_string(const string &str, const char c)
{
    vector<string> tokens;
    string word;
    for (const auto &len : str) {
        if (len == c && word.size()) {
            tokens.push_back(word);
            word.clear();
        } else if (len != c) {
            word += len;
        }
    }

    if(word.size()) {
        tokens.push_back(word);
    }

    return tokens;
}

/*
 * Removes leading and trailing whitespace characters from
 * the passed string.
 */
string& trim(string &str)
{
    auto isspace = [](char ch) {
        /* NOTE: should we specify the locale? */
        return !std::isspace(ch);
    };

    /* Strip leading whitespace.. */
    auto start = std::find_if(str.cbegin(), str.cend(), isspace);
    str.erase(str.cbegin(), start);

    /* .. and trailing. */
    auto end = std::find_if(str.crbegin(), str.crend(), isspace);
    str.erase(end.base(), str.cend());

    return str;
}

/*
 * Akin to Pythons join: concatenate a vector of strings
 * with intervening occurrences of sep.
 */
string join(const vector<string> &v, const string &sep)
{
    string retstr = "";
    for (const auto &str : v)
        retstr += str + (str == v.back() ? "" : sep);

    return retstr;
}

/*
 * Process the string by
 *  - replace non-alphanumeric characters with whitespace,
 *  - trim whitespace, and
 *  - forcing to lower case.
 */
string full_process(string str)
{
    /* Replace non-alphanumeric characters with whitespace, */
    std::replace_if(str.begin(), str.end(), [](char ch) {
        /* NOTE: same thing here: specify locale? */
        return !std::isalnum(ch);
    }, ' ');

    /* trim whitespace, and */
    str = utils::trim(str);

    /* force to lower case. */
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    return str;
}

/*
 * std::min, but for size_t.
 */
size_t min(size_t a, size_t b)
{
    return a < b ? a : b;
}

}  // ns utils

}  // ns fuzz
