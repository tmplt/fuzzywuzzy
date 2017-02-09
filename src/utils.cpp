#include "utils.hpp"
#include "common.hpp"

#include <algorithm>
#include <cmath>

namespace utils {

// use remove_suffix

vector<string> split_string(const string_view &str, const char c)
{
    vector<string> result;

    /* NOTE: breaks for strings missing c. */
    for (string_view::const_iterator len = str.begin(); len <= str.end(); len++) {
        string_view::const_iterator token_start = len;

        while (*len != c && *len)
            len++;

        result.emplace_back(token_start, len);
    }

    return result;
}

string_view trim(const string_view &str, const string_view &whitespace = " \t")
{
    const auto begin = str.find_first_not_of(whitespace);
    if (begin == string_view::npos)
        return "";

    const auto end = str.find_last_not_of(whitespace);
    const auto length = end - begin + 1;

    return str.substr(begin, length);
}

string trim(string &str, const string &whitespace = " \t")
{
    /* Find first char where prefixed whitespace is trimmed. */
    auto begin = str.find_first_not_of(whitespace);

    if (begin == string::npos) {
        /* The string is empty, or only contains spaces. */
        return "";
    }

    /* Last char where suffixed whitespace is trimmed. */
    auto end = str.find_last_not_of(whitespace);

    auto length = end - begin + 1;
    return str.substr(begin, length);
}

string& clean_string(string &s)
{
    if (s.empty()) {
        /*
         * There is nothing to do;
         * the string is as clean as could be.
         */
        return s;
    }

    /*
     * Keep only letters and numbers.
     * Substitute invalid characters with a space.
     */
    auto is_invalid_char = [](const char c) {
        return !std::isalpha(c) && !std::isdigit(c);
    };

    std::replace_if(s.begin(), s.end(), is_invalid_char, ' ');

    /*
     * Lowercase the whole string.
     * NOTE: This might break with non-ASCII.
     */
    //std::transform(s.begin(), s.end(), std::tolower);

    s = trim(s);
    return s;
}

int percrnd(double val)
{
    return static_cast<int>(std::round(100 * val));
}

}
