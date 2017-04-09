This is an in-progress port of [seatgeek's fuzzywuzzy](https://github.com/seatgeek/fuzzywuzzy/) Python library to C++.
When done, this library will have the same interface and behavior.

The underlaying C-library ([python-Levenshtein](https://github.com/miohtama/python-Levenshtein), mirrored [here](https://github.com/Tmplt/python-Levenshtein)) has been stripped of its Python interfacing
and been wrapped around some C++ code.

| files in `src/` | Python/C-lib equivalent |
| ----- | ----------------------- |
| `fuzzywuzzy.{c,h}pp` and `string_matcher.{c,h}pp` | Line-by-line Python-to-C++ translations of the Python library and python-Levenshtein's `StringMatcher.py`. |
| `wrapper.{c,h}pp` | (Python-interfaced-)C-to-C++ wrapper of `ratio_py`, `get_opcodes_py`, `get_matching_blocks_py`, etc. from python-Levenshtein. |
| `utils.{c,h}pp` | Utility functions, translated from the Python library's `utils.py`. |
| `levenshtein.{c,h}` | The underlaying C functions, copied verbatim. |

Usage
-----
```cpp
#include <fuzzywuzzy>
```

**Simple Ratio**
```cpp
fuzz::ratio("this is a test", "this is a test!"); // returns 97
```

**Partial Ratio**
```cpp
fuzz::partial_ratio("this is a test", "this is a test!"); // return 100
```

**Token Sort Ratio**
```cpp
fuzz::ratio("fuzzy wuzzy was a bear", "wuzzy fuzzy was a bear"); // returns 91

fuzz::token_sort_ratio("fuzzy wuzzy was a bear", "wuzzy fuzzy was a bear"); // returns 100
```

**Token Set Ratio**
```cpp
fuzz::token_sort_ratio("fuzzy was a bear", "fuzzy fuzzy was a bear"); // returns 83 (this should be 84)

fuzz::token_set_ratio("fuzzy was a bear", "fuzzy fuzzy was a bear"); // returns 100
```