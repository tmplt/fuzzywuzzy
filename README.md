This is an in-progress port of @seatgeek's [fuzzywuzzy](https://github.com/seatgeek/fuzzywuzzy/) Python library to C++.
This library will have the same interface as the Python library.

The underlaying C-library ([python-Levenshtein](https://github.com/miohtama/python-Levenshtein),
mirrored [here](https://github.com/Tmplt/python-Levenshtein)) is used with a C++-wrapper.

Progress
---
Simple and advanced scoring functions:

- [X] `ratio()`
- [X] `partial_ratio()`
- [ ] `token_sort_ratio()`
- [ ] `partial_token_sort_ratio()`
- [ ] `token_set_ratio()`
- [ ] `partial_token_set_ratio()`

Combination API:

- [ ] `QRatio()`
- [ ] `WRatio()`
- [ ] `UWRatio()`
