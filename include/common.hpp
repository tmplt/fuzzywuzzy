#pragma once

#include <string>
#include <vector>

#ifdef __clang__
    #include <string_view>
    using std::string_view;
#else
    #include <experimental/string_view>
    using std::experimental::string_view;
#endif

using std::vector;
using std::string;
