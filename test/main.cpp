#include <iostream>
#include "fuzzywuzzy.hpp"

int main()
{
    const string a = "I'm in your mind", b = "I'm in your mind fuzz";

    std::cout << fuzz::ratio(a, b) << '\n';
    std::cout << fuzz::partial_ratio(a, b) << '\n';
}
