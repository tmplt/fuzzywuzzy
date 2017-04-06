#include "utils.hpp"
#include "common.hpp"

#include <algorithm>
#include <cmath>

namespace utils {

/*
 * percent-rounded: return an int in the range [0,100].
 * TODO: make up a new name.
 */
int percrnd(double val)
{
    return static_cast<int>(std::round(100 * val));
}

/*
 * Split a string into multiple strings when a character is met.
 * Returns all tokens in an array.
 */
vector<string> split_string(const string &str, const char c = ' ')
{
    vector<string> tokens;

    for (string::const_iterator len = str.begin(); len <= str.end(); len++) {
        string::const_iterator token_start = len;

        while (*len != c && *len)
            len++;

        tokens.emplace_back(token_start, len);
    }

    return tokens;
}

}
