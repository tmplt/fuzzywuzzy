#include <iostream>
#include "fuzzywuzzy.hpp"

int main()
{
    std::cout << fuzz::ratio("åäö", "å") << '\n';
}
