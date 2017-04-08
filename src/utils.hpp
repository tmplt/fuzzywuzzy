#pragma once

#include "common.hpp"

#include <algorithm>  // std::min_element()

namespace utils {

int percent_round(double val);
vector<string> split_string(const string &str, const char c = ' ');
string& trim(string &str);
string join(const vector<string> &v, const string &sep = " ");
string full_process(string str);
size_t max(size_t a, size_t b);

}
