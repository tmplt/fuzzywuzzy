#pragma once

#include "common.hpp"

#include <algorithm>  // std::min_element()

namespace utils {

int percent_round(double val);
vector<string> split_string(const string &str, const char c = ' ');
string& trim(string &str);

}
