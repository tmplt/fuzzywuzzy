This is an in-progress port of @seatgeek's [fuzzywuzzy](https://github.com/seatgeek/fuzzywuzzy/) python library to C++.
This library will have the same interface as the Python library, but if I'm able I'll see about C++-ifying the underlaying C code (see `algorithms::levenshtein_distance`).
When this is done, it should behave _exactly_ like the Python library.

Progress
---
Simple and advanced scoring functions:

- [X] `ratio()`
    - [X] `levenshtein_distance()`
- [X] `partial_ratio()`
    - [X] `string_matcher::get_matching_blocks()`
- [ ] `token_sort_ratio()`
- [ ] `partial_token_sort_ratio()`
- [ ] `token_set_ratio()`
- [ ] `partial_token_set_ratio()`

Combination API:

- [ ] `QRatio()`
- [ ] `WRatio()`
- [ ] `UWRatio()`
