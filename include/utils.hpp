#pragma once

#include "common.hpp"

#include <algorithm>  // std::max_element()

namespace fuzz {

namespace utils {

unsigned int percent_round(double val);

unsigned int intr(double val);

vector<string> split_string(const string &str, const char c = ' ');

string& trim(string &str);

string join(const vector<string> &v, const string &sep = " ");

string full_process(string str);

size_t min(size_t a, size_t b);

#ifdef CPP17

template <typename First, typename ... T>
decltype(auto) max(const First &f, const T & ... t)
{
    const First *retval = &f;
    ( (retval = &std::max(*retval, t)), ... );
    return *retval;
}

#else

/*
 * An "extension" of std::max() so that more than two arguments
 * can be passed. The first argument decides what everything else
 * is casted too.
 *
 * Hopefully the compiler will complain if we pass this something stupid.
 * NOTE: Can this be done when omitting first?
 */
template <typename T, typename... Args>
auto max(const T &first, const Args&... args)
{
    std::vector<T> vec = {first, static_cast<T>(args)...};
    auto max = std::max_element(vec.cbegin(), vec.cend());

    return *max;
}

#endif

}  // utils utils
  
}  // utils fuzz
