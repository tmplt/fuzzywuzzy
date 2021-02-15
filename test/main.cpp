#include <iostream>

#include "fuzzywuzzy.hpp"
#include "process.hpp"

int main()
{
    const std::string a = "I'm in your mind", b = "I'm in your mind fuzz";
    const std::string c = "fuzzy wuzzy was a bear", d = "wuzzy fuzzy was a bear";

    std::cout << fuzz::ratio(a, b) << '\n';
    std::cout << fuzz::partial_ratio(a, b) << '\n';
    std::cout << fuzz::token_sort_ratio(c, d) << '\n';

    std::vector<string> v = {"fuzzy", "wuzzy", "wuzzy", "fuzzy", "fuzzy", " "};
    auto erg = fuzz::dedupe(v);
}
