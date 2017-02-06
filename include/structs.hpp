#pragma once

enum edit_type {
    erase,  // since "delete" is a keyword.
    equal,
    insert,
    replace,
    keep
};

struct edit_op {
    edit_type type;
    int spos;  // source block position
    int dpos;  // destination block position
};

struct matching_block {
    int spos;  // source position
    int dpos;  // destination position
    int length;
};

struct op_code {
    edit_type type;
    int sbeg, send;
    int dbeg, dend;
};

